# https://github.com/tch0/MyConfigurations/blob/master/MakefileTemplate/CppTemplate1.mk

# Makefile template 1:
# For C++ sigle files, every C++ file will get a executable.

# make debug=yes to compile with -g
# make system=windows for windows system

.PHONY : all run_all_tests run_all_tests_in_detail run_all_eff_tests run_all_eff_tests_in_detail
.PHONY .IGNORE : clean

# add you own include path/library path/link library to CXXFLAGS
CXX = g++
CXXFLAGS += -I../include
CXXFLAGS += -std=c++20 # use C++20 standard
RM = rm

# debug
ifeq ($(debug), yes)
CXXFLAGS += -g
else
CXXFLAGS += -O3
CXXFLAGS += -DNDEBUG
endif

#========================================================================================================
#================================== Customize here ======================================================
#========================================================================================================
# add all test targets here
all_test_targets += TestMemory
all_test_targets += TestIterator
all_test_targets += TestSequentialContianers
all_test_targets += TestContainerAdaptors
all_test_targets += TestTreeImpl
all_test_targets += TestAssociativeContainers
all_test_targets += TestHashtableImpl
all_test_targets += TestUnorderedContainers
all_test_targets += TestAlgorithms

# add all efficiency test targets here
all_efficiency_test_targets += 
#========================================================================================================
# all targets
all_targets += $(all_test_targets)
all_targets += $(all_efficiency_test_targets)

# all
all : $(all_targets)

# compile every single targets
% : %.cpp
	$(CXX) $^ -o $@ $(CXXFLAGS)

# run every single test
$(addprefix run_single_target_, %) : %
	-./$^
$(addprefix run_single_target_in_detail_, %) : %
	-./$^ -d
run_all_test_targets := $(addprefix run_single_target_, $(all_test_targets))
run_all_test_targets_in_detail := $(addprefix run_single_target_in_detail_, $(all_test_targets))
run_all_efficiency_test_targets := $(addprefix run_single_target_, $(all_efficiency_test_targets))
run_all_efficiency_test_targets_in_detail := $(addprefix run_single_target_in_detail_, $(all_efficiency_test_targets))

# targets of running
run_all_tests : $(run_all_test_targets)
run_all_tests_in_detail : $(run_all_test_targets_in_detail)
run_all_eff_tests : $(run_all_efficiency_test_targets)
run_all_eff_tests_in_detail : $(run_all_efficiency_test_targets_in_detail)

# system: affect how to clean and executable file name
# value: windows/unix
system = unix
ifeq ($(system), windows)
all_targets := $(addsuffix .exe, $(all_targets))
RM := del
endif

# clean
clean :
	-$(RM) $(all_targets)
