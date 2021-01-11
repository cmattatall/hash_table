Source modules that are placed in this (or sub-directories) Will be added
to the test tree. Tests are not normally built unless the current
cmake project is also the top-level cmake project. 

The CLI definition for BUILD_TESTING=ON can be provided to 
build tests when this project is included in a larger cmake project.

These source modules should be a single file executable translation unit
whose mainline returns 0 on success and a non-zero code on failure.
