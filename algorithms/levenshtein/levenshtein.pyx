# distutils: language = c++
# distutils: sources = LevenshteinSimilarity.cpp
 
from libcpp.string cimport string 
from libcpp.memory cimport unique_ptr, allocator

cdef extern from "LevenshteinSimilarity.h":
    cdef double levenshtein_ratio(string str1, string str2)

cpdef similarity_ratio(str pystr1, str pystr2):
    cdef:
        string str1 = pystr1.encode('utf-8')
        string str2 = pystr2.encode('utf-8')
        double ratio = levenshtein_ratio(str1, str2)
    return ratio 