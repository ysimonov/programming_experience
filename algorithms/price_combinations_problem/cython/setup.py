try:
    from setuptools import setup
    from setuptools import Extension
except ImportError:
    from distutils.core import setup
    from distutils.extension import Extension

from Cython.Build import cythonize

# build:
# python3 setup.py build_ext --inplace

setup(
    author="Yevgeniy Simonov",
    description="Price combination problem",
    ext_modules=cythonize(
        Extension(
            name="price_combination_finder",
            sources=["price_combination_finder.pyx"],
            extra_compile_args=["-O2"]
        ),
        annotate=True,
        compiler_directives={'language_level' : "3"}
    ),
    zip_safe=True
)