CONFIG ?= development
### many variables are stored in a shared makefile
include Makefile.common
include Makefile.misc
# build parameters
ifeq "$(CONFIG)" "development"
	DBGFLAGS := -g
	CFLAGS := $(OFLAG) $(DBGFLAGS) $(STDFLAG) $(INCFLAGS) $(WFLAGS)
endif
ifeq "$(CONFIG)" "release"
	OFLAG := -O0
	CFLAGS := $(OFLAG) $(INCFLAGS)
endif
ifeq "$(OS)" "Alpine"
	CFLAGS := $(DBGFLAGS) $(INCFLAGS)
endif
# CFLAGS ?= $(OFLAG) $(DBGFLAGS) $(STDFLAG) $(INCFLAGS) $(WFLAGS)
# Makefile search paths
vpath %.c $(SRC) $(UTL)
vpath lib%.a $(LIB)
export


.PHONY: all
all:
ifndef OS
	$(warning warning: it looks like this Makefile is not fully adapted to your OS)
endif
	$(MAKE) lib
ifdef LIB_CHECK
	$(MAKE) test
ifdef GCOVR
	$(MAKE) gcov_report
endif
ifdef VALGRIND
	$(MAKE) memcheck
endif
endif

$(PROJNAME).a: $(OBJS)
	ar -rsc $@ $^
	cp $(INC)/$(LIBNAME).h .

$(OBJ)/%.o: %.c $(HDRS) | $(OBJ)
	$(CC) -c $(CFLAGS) -o $@ $<
# directory creation
$(OBJ):
	@$(MK) $@

.PHONY: test retest gcov_report show
retest: fclean test
test gcov_report show: lib
	@$(MAKE) -s -f Makefile.unit_testing $@

.PHONY: clean
clean:
	$(RM) $(BUILD)

.PHONY: fclean# full clean
fclean: clean
	$(RM) $(REPORTS)
	$(RM) $(LIBNAME).a
	$(RM) $(LIBNAME).h

.PHONY: re
re: fclean
	$(MAKE) all


# helper targets
.PHONY: check
check:
	command -v 

.PHONY: prep
prep:
ifeq "$(KERNEL)" "Linux"
	sudo apt install -y pkg-config check gcovr
endif
ifeq "$(KERNEL)" "Darwin"
	yes | brew install pkg-config check gcovr
endif

.PHONY: lib
lib: lib$(PROJNAME).a
lib$(PROJNAME).a: $(PROJNAME).a
	$(MK) $(LIB)
	cp $< $(LIB)/$@

.PHONY: memcheck
memcheck: lib
ifndef VALGRIND
	$(error "valgrind" should be installed)
endif
	@$(MAKE) -s -f Makefile.memcheck $@

.PHONY: san
san: lib
ifeq "$(ARCHITECTURE)" "arm64"
	$(error adress sanitizers work properly only on x86-64 at the time)
endif
	@$(MAKE) -s -f Makefile.sanitizers $@

.PHONY: linter_check linter_fix
linter_fix:   LINTFLAGS := -i $(LINTFLAGS)
linter_check: LINTFLAGS := -n $(LINTFLAGS)
linter_check linter_fix:
	$(LINT) $(LINTFLAGS) $(shell find . -type f -name '*.h' -o -name '*.c')

.PHONY: tidy
tidy:
	clang-tidy $(shell find . -type f -name '*.c') -- -I$(INC) -I$(HDR)

.PHONY: miniverter
miniverter: fclean
	cd $(SCRIPTS_DIR) && bash run_miniverter_test.sh

.PHONY: container
container:
ifndef DOCKER
	$(error "docker" should be installed)
endif
	$(MAKE) fclean
	bash $(SCRIPTS_DIR)/run_docker_image.sh

.PHONY: brew
brew:
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
