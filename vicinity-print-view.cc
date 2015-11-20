#include "vicinity.hh"
#include "dataset.hh"

#include <boost/range/algorithm/set_algorithm.hpp>
#include <boost/function_output_iterator.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iomanip>
#include <set>

using boost::make_function_output_iterator;
using boost::lambda::var;
using boost::lambda::_1;
using boost::copy;
using std::setw;
using std::cout;
using std::endl;

template<typename RPS>
void vicinity<RPS>::print_view() const {
    copy(
	view | ::helpers::map_ids,
	make_function_output_iterator(
	    var(cout) << setw(3) << _1 << ",") // another sol here: http://mariusbancila.ro/blog/2008/04/10/output-formatting-with-stdcopy/
	);
    cout << endl;
}

template<typename RPS>
double vicinity<RPS>::recall() const {
		std::vector<item_id_t> intersection;
		for(auto neighbor : view | ::helpers::map_ids)
				boost::set_intersection(dataset.value()[neighbor], dataset.value()[RPS::id], std::back_inserter<>(intersection));
		std::set<item_id_t> s{std::begin(intersection), std::end(intersection)};
		return s.size() / static_cast<double>(dataset.value()[RPS::id].size());
}

// explicit instantiation
#include "cyclon.hh"
template void vicinity<cyclon>::print_view() const;
template double vicinity<cyclon>::recall() const;
