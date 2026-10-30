# Security

## Reporting

Please report suspected vulnerabilities privately to the maintainers rather than
opening a public issue.

## Secret handling

- Never commit real credentials. Use `.env` (ignored) based on `.env.example`.
- Database connection strings are built through client library parameter APIs;
  passwords are not concatenated into unescaped strings.
- Inputs to the SQL parser are size- and NUL-validated at the entry point.

## Supported build

Run `make test` before submitting changes; CI runs the same command.