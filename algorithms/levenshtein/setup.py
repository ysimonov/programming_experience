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
            name="levenshtein",
            sources=["levenshtein.pyx", "LevenshteinSimilarity.cpp"],
        ),
        annotate=True,
        compiler_directives={'language_level' : "3"}
    ),
    zip_safe=True
)