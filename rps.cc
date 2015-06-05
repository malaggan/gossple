#include "rps.hpp"

#include "RandomSample.tcc"
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <iostream>
#include <iterator>
#include <cassert>
#include <iostream>
#include <iomanip>
RPS::RPS(user_id_t me, std::unordered_set<RPS*> &already_joined) 
    : abstract_user{me}, view{}
{	
    auto bootstrapPeers = RandomSample(already_joined, viewSize);
    already_joined.insert(this);
    for(auto const& other : bootstrapPeers)
    {
	// if the other's RPS view is incomplete, swap selves:
	if(other->getView().size() < viewSize)
	{
	    view[other->id] = 0; // 0 is the age
	    other->getView()[id] = 0; 
	}
	else
	{
	    auto rps_other = dynamic_cast<RPS*>(other);
	    assert(rps_other != nullptr);
	    auto removed = rps_other->RandomReplace(id);
	    if(view.find(removed) != std::end(view))
		view[other->id] = 0;
	    else 
		view[removed] = 0;
	}
    }
}

auto RPS::RandomNeighbor() const -> user_id_t {
    return *RandomSample(view | boost::adaptors::map_keys, 1).begin();
}
#include <boost/function_output_iterator.hpp>
#include <boost/lambda/lambda.hpp>
void RPS::printView() const {
    using namespace boost::lambda;
    boost::copy(
	view | boost::adaptors::map_keys,
	//std::ostream_iterator<int>(std::cout, ",")
	boost::make_function_output_iterator( 
	    var(std::cout) << std::setw(3) << _1 << ",") // another sol here: http://mariusbancila.ro/blog/2008/04/10/output-formatting-with-stdcopy/
	);
    std::cout << std::endl;
}

auto RPS::RandomReplace(user_id_t id) -> user_id_t {
    auto victim = RandomNeighbor();
    view.erase(victim);
    view[id] = 0;
    return victim;
}
