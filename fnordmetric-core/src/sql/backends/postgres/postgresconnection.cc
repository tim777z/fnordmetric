/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <fnordmetric/environment.h>
#include <fnordmetric/sql/backends/postgres/postgresconnection.h>
#include <iostream>
#include <sstream>
#include <string>

namespace fnordmetric {
namespace query {
namespace postgres_backend {

std::unique_ptr<PostgresConnection> PostgresConnection::openConnection(
    const util::URI& uri) {
  std::unique_ptr<PostgresConnection> conn(new PostgresConnection());
  conn->connect(uri);
  return conn;
}

PostgresConnection::PostgresConnection() : postgres_(nullptr) {
#ifndef FNORD_ENABLE_POSTGRES
  RAISE(kRuntimeError, "FnordMetric was compiled without libpostgresclient");
#endif
}

PostgresConnection::~PostgresConnection() {
#ifdef FNORD_ENABLE_POSTGRES
  if (postgres_)
    PQfinish(postgres_);
#else
  RAISE(kRuntimeError, "FnordMetric was compiled without libpostgresclient");
#endif
}

void PostgresConnection::connect(const util::URI& uri) {
  unsigned int port = 5432;
  std::string host = uri.host();
  std::string username;
  std::string password;
  std::string database;

  if (host.size() == 0) {
    RAISE(
        kRuntimeError,
        "invalid postgres:// URI: has no hostname (URI: '%s')",
        uri.toString().c_str());
  }

  if (uri.port() > 0) {
    port = uri.port();
  }

  if (uri.path().size() < 2 || uri.path()[0] != '/') {
    RAISE(
        kRuntimeError,
        "invalid postgres:// URI: missing database, format is: postgres://host/db "
        " (URI: %s)",
        uri.toString().c_str());
  }

  database = uri.path().substr(1);

  for (const auto& param : uri.queryParams()) {
    if (param.first == "username" || param.first == "user") {
      username = param.second;
      continue;
    }

    if (param.first == "password" || param.first == "pass") {
      password = param.second;
      continue;
    }

    RAISE(
        kRuntimeError,
        "invalid parameter for postgres:// URI: '%s' (value redacted)",
        param.first.c_str());
  }

  connect(host, port, database, username, password);
}

void PostgresConnection::connect(
    const std::string& host,
    unsigned int port,
    const std::string& database,
    const std::string& username,
    const std::string& password) {
#ifdef FNORD_ENABLE_POSTGRES
  // Build the connection using libpq's parameter API rather than concatenating
  // an unescaped connection string, which could leak or mis-handle credentials.
  std::string port_str = std::to_string(port);
  const char* const keys[] = {
      "hostaddr", "port", "dbname", "user", "password", nullptr};
  const char* const values[] = {
      host.c_str(),
      port_str.c_str(),
      database.empty() ? nullptr : database.c_str(),
      username.empty() ? nullptr : username.c_str(),
      password.empty() ? nullptr : password.c_str(),
      nullptr};

  postgres_ = PQconnectdbParams(keys, values, 0);
  if (PQstatus(postgres_) != CONNECTION_OK) {
    RAISE(
      kRuntimeError,
      "PQconnectdbParams() failed: %s\n",
      PQerrorMessage(postgres_));
  }
#else
  RAISE(kRuntimeError, "FnordMetric was compiled without libpostgresclient");
#endif
}


std::vector<std::string> PostgresConnection::describeTable(
    const std::string& table_name) {
  std::vector<std::string> columns;

#ifdef FNORD_ENABLE_POSTGRES
  // construct attribute names query (escape the table name to avoid SQL injection)
  if (table_name.empty()) {
    RAISE(kRuntimeError, "describeTable: empty table name");
  }
  std::stringstream query;
#ifdef FNORD_ENABLE_POSTGRES
  char* escaped = PQescapeIdentifier(postgres_, table_name.c_str(), table_name.size());
  if (escaped == nullptr) {
    RAISE(kRuntimeError, "PQescapeIdentifier failed: %s", PQerrorMessage(postgres_));
  }
  query << "select attname from pg_attribute join pg_class on attrelid = pg_class.oid where relname = ";
  query << escaped;
  PQfreemem(escaped);
  query << " and attnum > 0";
#else
  query << "select attname from pg_attribute join pg_class on attrelid = pg_class.oid where relname = '";
  query << table_name;
  query << "' and attnum > 0";
#endif
  
  // run query
  PGresult* res = PQexec(postgres_, query.str().c_str());
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    abort();
    RAISE(
      kRuntimeError,
      "PQexec() failed: %s\n",
      PQerrorMessage(postgres_));
  }

  // retrieve result and add to columns vector
  auto num_rows = PQntuples(res);
  for (int i = 0; i < num_rows; ++i) {
    columns.emplace_back(PQgetvalue(res,i,0));
  }

  PQclear(res);
#else
  RAISE(kRuntimeError, "FnordMetric was compiled without libpostgresclient");
#endif
  return columns;
}

void PostgresConnection::executeQuery(
    const std::string& query,
    std::function<bool (const std::vector<std::string>&)> row_callback) {
#ifdef FNORD_ENABLE_POSTGRES
  if (env()->verbose()) {
    fnord::util::LogEntry entry;
    entry.append("__severity__", "DEBUG");
    entry.append("__message__", "Executing Postgres query");
    entry.append("query", query);
    env()->logger()->log(entry);
  }

  // run query
  PGresult* res = PQexec(postgres_, query.c_str());
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    RAISE(
      kRuntimeError,
      "PQexec() failed: %s\n",
      PQerrorMessage(postgres_));
  }

  // retrieve result and add to columns vector
  auto num_rows = PQntuples(res);
  for (int r = 0; r < num_rows; ++r) {
    auto num_cols = PQnfields(res);
    std::vector<std::string> row_vec;
    for (int c = 0; c < num_cols; ++c) {
      row_vec.emplace_back(PQgetvalue(res,r,c));
    }
    
    try {
      if (!row_callback(row_vec)) {
        break;
      }
    } catch (const std::exception& e) {
      PQclear(res);
      try {
        auto rte = dynamic_cast<const util::RuntimeException&>(e);
        throw rte;
      } catch (std::bad_cast bce) {
        throw e;
      }
    }
  }

  PQclear(res);
#else
  RAISE(kRuntimeError, "FnordMetric was compiled without libpostgresclient");
#endif
}

}
}
}
