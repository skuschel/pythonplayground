
#include <boost/python.hpp>


char const* greet()
{
   return "hello, world";
}

double sum(double x, double y)
{
    return x+y;
}

double sumarray(boost::python::list& list)
{
    double ret = 0;
    int n = len(list);
    for (int i = 0; i < n; i++){
        ret += boost::python::extract<double>(list[i]);
    }
    return ret;
}

BOOST_PYTHON_MODULE(hello_ext)
{
    using namespace boost::python;
    def("greet", greet);
    def("sum", sum);
    def("sumarray", sumarray);
}

