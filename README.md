# mkiptables

A simple tool to build Linux-based NAT routers with a single configuration file.

Author: Maxthon Chan &lt;<xcvista@me.com>&gt;

## Compiling this tool

To compile this tool, go to `mkiptables/` folder and `make` it. No configuration
is needed.

## Using this tool

This tool dumps its output to stdout for the ease of piping.

Using this tool is really simple, as you just pipe `mkiptables` output into
`iptables-restore` and it should be okay.

## More about this tool

### `nftables`

`nftables` support is planned but it will only be implemented after the first
Ubuntu release that supports this feature is released.

### IPv6

> Who uses NAT with IPv6?

Currently no IPv6 support is planned.

### About the Xcode project files

This tool is written partly under OS X with Xcode, for its superior code sensing
and code completion support. The code is written in only ISO C99 and is deployed
in some semi-production environment.
