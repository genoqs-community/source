# Octopus & Nemo Community Edition OS
Hello and welcome! We are a small team developing software refinements and stability updates for these epic hardware sequencers.
## Contributing
If you would like to contribute please get in touch on the GenoQs group and we can help you get going.
### Prerequisites
* Octopus or Nemo ;)
* Development Environment (see Getting Started)
* Eclipse
* Openocd
* Optional ARM JTAG
### Getting Started
For instructions on how to manually setup your environment please read Manual Environment Setup (Note also tested on Ubuntu 10) If you prefer to skip that and just get a VM image please ask on the GenoQs Users Group
Branches
### Feature Code isolation
When writing a new feature and you are adding to an existing file wrap the inserted block in a define wrapper i.e. 
```
#ifdef SOMENAME_FEATURE_ENABLE
// Feature code
#endif
```
You can define the feature inside versions.h
```
#define SOMENAME_FEATURE_ENABLE
```
By commenting out this line above you will be able to 100% omit the feature code from the compile.
### Merging
When your feature is ready to merge, please get in touch with a build and branch name so we may review before merging.
## License
```
GNU GENERAL PUBLIC LICENSE

Version 2, June 1991

Copyright (C) 1989, 1991 Free Software Foundation, Inc.
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA

Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
Acknowledgments

    GenoQs Machines
```
