try:
    from setuptools import setup
    from setuptools import Extension
except ImportError:
    from distutils.core import setup
    from distutils.extension import Extension

from Cython.Build import cythonize
from matplotlib.pyplot import annotate

# build:
# python3 setup.py build_ext --inplace

setup(
    author="Yevgeniy Simonov",
    description="Calculate similarity ratio between two strings using Levenshtein distance metric",
    ext_modules=cythonize(
        Extension(
            name="price_combination_finder",
            sources=["price_combination_finder.pyx", "priceCombinationFinder.cpp"],
            extra_compile_args=["-std=c++11"]
        ),
        annotate=True,
        compiler_directives={'language_level' : "3"}
    ),
    zip_safe=True
)