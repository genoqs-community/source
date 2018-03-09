# Octopus & Nemo Community Edition OS

Hello and a very warm welcome to the CE OS Wiki! We are a small team dedicated to the refinement of these sequencers that we adore. Our focus is especially trained on bug fixes, stability and features that complement or facilitate work flow around the core design.

We keep a spreadsheet of issues and features Feature / Issue Cases

We also maintain a known Issues / CBB (could be better) document for future consideration, the pdf is available here Known Issues
Infinity & Beyond!

Although stability is our central concern we would to accommodate brilliant future feature ideas safely within the codebase. We want to support the concept in the future of optional Addon Feature 'flavours', without compromising or over caveating the Core OS.
Here are the definitions of what is Core and what is Addon:
Core Features

Core features are what we regard as running in parallel or as a complementary reflection of the original design. This can include: - UI / UX Work flow features and tweaks to UI or input response.
- Extensions of current features where limits lifts are justifable, always in keeping and consistent with the feature / global UI schema
Addon Features

Features / ideas that fall outside the core feature categories that may perhaps have an impact in terms of resource / CPU requirements. Also these features should follow the established UI vernacular and feel like a natural fit.
Getting Started

For instructions on how to manually setup your environment please read Manual Environment Setup (Note also tested on Ubuntu 10) If you prefer to skip that and just get a VM image please ask on the GenoQs Users Group
Branches

Master branches for CE OS to clone octopus_ce_master nemo_ce_master
Contributing

If you would like to contribute please get in touch on the GenoQs group and we can help you get setup.
Feature Code isolation

When writing a new feature and you are adding to an existing file wrap the inserted block in a define wrapper i.e. ```
ifdef SOMENAME_FEATURE_ENABLE

// Feature code
endif

You can define the feature inside versions.h
define SOMENAME_FEATURE_ENABLE

``` By commenting out this line above you will be able to 100% omit the feature code from the compile.
Merging

When your feature is ready to merge, please get in touch with a build and branch name so we may review before merging.
Prerequisites

    Octopus or Nemo ;)
    Development Environment (see Getting Started)
    Eclipse
    Openocd
    Optional ARM JTAG

License

GNU GENERAL PUBLIC LICENSE

Version 2, June 1991

Copyright (C) 1989, 1991 Free Software Foundation, Inc.
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA

Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
Acknowledgments

    GenoQs Machines
