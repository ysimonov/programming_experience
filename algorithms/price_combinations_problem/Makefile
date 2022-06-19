PACKAGENAME=Cython
PYTHON?=python3

# to build all cython files, run: make cython-build

dir := .

# rm -f $(addsuffix /*.cpp,$(dir))
# rm -f $(addsuffix /*.c,$(dir))
# rm -f $(addsuffix /*.html,$(dir))

clean-all:
	rm -f $(addsuffix /*.so,$(dir))
	rm -f $(addsuffix /*.pyd,$(dir))
	rm -f $(addsuffix /*.py[co],$(dir))

.PHONY: cython-build
cython-build: clean-all
	${PYTHON} $(addsuffix /setup.py,$(dir)) build_ext --build-lib $(addsuffix /,$(dir))