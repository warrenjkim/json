# Architecture Migration

* Moving away from pure visitors, and into typed unions. This is because it is
  such a pain in the ass to use bazel with the current structure. There are
  also way too many messy dependencies that CMake hid.
* Migrating to bazel as a build system. Seems interesting and it looks
  better/easier to use than CMake.
