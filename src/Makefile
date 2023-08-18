CONFIG ?= submission
### many variables are stored in a shared makefile
include Makefile.common
include Makefile.misc
# build parameters
ifeq "$(CONFIG)" "testing"
	CFLAGS := $(DBGFLAGS) $(STDFLAG) $(INCFLAGS) $(WFLAGS)
endif
ifeq "$(CONFIG)" "submission"
	CFLAGS := $(DBGFLAGS) $(INCFLAGS)
endif
ifeq "$(CONFIG)" "release"
	CFLAGS := $(OFLAG) $(INCFLAGS)
endif
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

.PHONY: test retest gcov_report recov show
retest: fclean test
recov: retest gcov_report
test gcov_report show:
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
