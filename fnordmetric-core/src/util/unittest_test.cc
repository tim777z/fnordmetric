/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <fnordmetric/util/runtimeexception.h>
#include <fnordmetric/util/unittest.h>

using namespace fnordmetric::util;

UNIT_TEST(UnittestTest);

TEST_CASE(UnittestTest, TestExpectedException, [] () {
  EXPECT_EXCEPTION("expected failure", [] () {
    RAISE(kRuntimeError, "expected failure");
  });
});

TEST_CASE(UnittestTest, TestUnexpectedExceptionDiagnostic, [] () {
  bool raised = false;

  try {
    EXPECT_EXCEPTION("expected failure", [] () {
      RAISE(kRuntimeError, "actual failure");
    });
  } catch (const RuntimeException& e) {
    raised = true;
    EXPECT(e.getMessage().find("actual failure") != std::string::npos);
  }

  EXPECT(raised);
});
