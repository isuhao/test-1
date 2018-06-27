//  (C) Copyright Gennadiy Rozental 2011-2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// @brief tests monomorphic join
// ***************************************************************************

// Boost.Test
#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic/join.hpp>
#include <boost/test/data/monomorphic/singleton.hpp>
#include <boost/test/data/monomorphic/array.hpp>
#include <boost/test/data/monomorphic/collection.hpp>
#include <boost/test/data/monomorphic/fwd.hpp>
#include <boost/test/data/for_each_sample.hpp>
namespace data=boost::unit_test::data;

#include "datasets-test.hpp"

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_mono_join )
{
    BOOST_TEST( (data::make( 1 ) + data::make( 5 )).size() == 2 );
    BOOST_TEST( (data::make( std::vector<int>(2) ) + data::make( std::list<int>(3) )).size() == 5 );

    int arr1[] = {1,2};
    int arr2[] = {7,19};
    BOOST_TEST( (data::make( arr1 ) + data::make( arr2 )).size() == 4 );

    BOOST_TEST( (data::make( 1 ) + data::make( 5 ) + data::make( 19 )).size() == 3 );

    BOOST_TEST( (data::make( std::vector<int>(2) ) + data::make( 1 ) + data::make( arr2 ) + data::make( 17 )).size() == 6 );

    int expected[] = {1,2,5,7,19,37};
    int* exp = expected;
    int c = 0;
    invocation_count ic;

    auto samples1 = data::make( std::vector<int>(arr1,arr1+2) ) + data::make( 5 ) + data::make( arr2 ) + data::make( 37 );

    ic.m_value = 0;
    data::for_each_sample( samples1, ic );
    BOOST_TEST( ic.m_value == 6 );

    c = 0;
    data::for_each_sample( samples1, [&c,exp](int i) {
        BOOST_TEST( i == exp[c++] );
    });

    auto samples2 = data::make( std::vector<int>(arr1,arr1+2) ) + (data::make( 5 ) + (data::make( arr2 ) + data::make( 37 )));

    ic.m_value = 0;
    data::for_each_sample( samples2, ic );
    BOOST_TEST( ic.m_value == 6 );

    c = 0;
    data::for_each_sample( samples2, [&c,exp](int i) {
        BOOST_TEST( i == exp[c++] );
    });

    auto samples3 = (data::make( std::vector<int>(arr1,arr1+2) ) + data::make( 5 )) + (data::make( arr2 ) + data::make( 37 ));

    ic.m_value = 0;
    data::for_each_sample( samples3, ic );
    BOOST_TEST( ic.m_value == 6 );

    c = 0;
    data::for_each_sample( samples3, [&c,exp](int i) {
        BOOST_TEST( i == exp[c++] );
    });

    copy_count::value() = 0;
    data::for_each_sample( data::make( copy_count() ) + data::make( copy_count() ), check_arg_type<copy_count>() );
    BOOST_TEST( copy_count::value() == 0 );

    copy_count::value() = 0;
    data::for_each_sample( data::make( copy_count() ) + data::make( copy_count() ) + data::make( copy_count() ), check_arg_type<copy_count>() );
    BOOST_TEST( copy_count::value() == 0 );

    copy_count::value() = 0;
    data::for_each_sample( data::make( copy_count() ) + (data::make( copy_count() ) + data::make( copy_count() )), check_arg_type<copy_count>() );
    BOOST_TEST( copy_count::value() == 0 );
    copy_count::value() = 0;
    data::for_each_sample( (data::make( copy_count() ) + data::make( copy_count() )) +
                           (data::make( copy_count() ) + data::make( copy_count() )), check_arg_type<copy_count>() );
    BOOST_TEST( copy_count::value() == 0 );

    auto ds1        = data::make( copy_count() );
    auto const ds2  = data::make( copy_count() );

    copy_count::value() = 0;
    data::for_each_sample( ds1 + ds1, check_arg_type<copy_count>() );
    BOOST_TEST( copy_count::value() == 0 );

    copy_count::value() = 0;
    data::for_each_sample( ds2 + ds2, check_arg_type<copy_count>() );
    BOOST_TEST( copy_count::value() == 0 );

    copy_count::value() = 0;
    data::for_each_sample( ds1 + ds2, check_arg_type<copy_count>() );
    BOOST_TEST( copy_count::value() == 0 );

    copy_count::value() = 0;
    auto jn1 = ds1 + data::make( copy_count() );
    data::for_each_sample( jn1, check_arg_type<copy_count>() );
    BOOST_TEST( copy_count::value() == 0 );

    copy_count::value() = 0;
    data::for_each_sample( data::make( copy_count() ) + ds1, check_arg_type<copy_count>() );
    BOOST_TEST( copy_count::value() == 0 );

    copy_count::value() = 0;
    data::for_each_sample( ds1 + ds2 + ds1, check_arg_type<copy_count>() );
    BOOST_TEST( copy_count::value() == 0 );

    copy_count::value() = 0;
    data::for_each_sample( ds1 + (ds1 + ds2), check_arg_type<copy_count>() );
    BOOST_TEST( copy_count::value() == 0 );

    copy_count::value() = 0;
    data::for_each_sample( (ds1 + ds1) + (ds2 + ds2), check_arg_type<copy_count>() );
    BOOST_TEST( copy_count::value() == 0 );

    copy_count::value() = 0;
    int std_vector_constructor_count = 0;
    {
      std::vector<copy_count> v(2); // we cannot do better than std::vector constructor
      std_vector_constructor_count = copy_count::value()/2;
    }

    copy_count::value() = 0;
    auto ds3 = data::make( make_copy_count_collection() ) + data::make( make_copy_count_collection() );
    BOOST_TEST( ds3.size() == 6 );
    data::for_each_sample( ds3, check_arg_type<copy_count>() );
    BOOST_TEST( copy_count::value() == std_vector_constructor_count * 2 * 3 );
}

//____________________________________________________________________________//


// faire un dataset iterator qui renvoit un objet temporaire.

// this dataset returns on purpose a temporary, and we want to check that the
// returned elements do not get released before the invocation from the join operation
// accessor.

template <class op_t, class value_t = int>
class fake_dataset {
public:
    enum { arity = 1 };
    typedef value_t sample;

    struct iterator {
        op_t op_;
        explicit    iterator( value_t&& value )
        : m_value(std::forward<value_t>(value))
        {}

        // forward iterator interface
        // The returned sample should be by value, as the operator* may return a temporary object
        decltype(std::declval<op_t>()(std::declval<value_t>()))
        operator*() const {
          return op_(m_value);
        }
        void operator++() {
          m_value++;
        }
      
        value_t m_value;
    };

    //fake_dataset( )
    //{}

    //! dataset interface
    data::size_t    size() const    { return 2; }
    iterator        begin() const   { return iterator(value_t()); } // default init the class
};

namespace boost { namespace unit_test { namespace data { namespace monomorphic {

template <class op_t, class value_t>
struct is_dataset< fake_dataset<op_t, value_t> > : mpl::true_ {};

}}}}

struct op_identity {

  template <class T>
  T const& operator()(T const& r) const {
      return r;
  }
};

struct op_copy {
  template <class T>
  T operator()(T const& r) const {
      return r;
  }
};

struct mono_invocation {
    mono_invocation() : m_value( 0 ) {}

    template<typename S>
    void    operator()( S const& ) const
    {
        m_value++;
    }

    mutable int    m_value;

private:
    mono_invocation(mono_invocation const&);
};

struct mono_check_current_number {
    mono_check_current_number() : m_value( 0 ) {}

    template<typename S>
    void    operator()( S const& s) const
    {
        assert(s.current_nb_instances > 0);
        m_value += s.current_nb_instances;
    }

    mutable int    m_value;

private:
    mono_check_current_number(mono_check_current_number const&);
};

struct counted_int {
  explicit counted_int() : value(0), has_moved(false) {
      current_nb_instances++;
  }
  
  counted_int(counted_int const& r) : value(r.value), has_moved(false) {
      current_nb_instances++;
  }

  counted_int(counted_int && r) : value(r.value), has_moved(false) {
    r.has_moved = true;
  }
  ~counted_int(){
      if(!has_moved)
          current_nb_instances--;
  }

  counted_int& operator++(int){
      value++;
      return *this;
  }

  int value;
  bool has_moved;
  static int current_nb_instances;
};
int counted_int::current_nb_instances = 0;


BOOST_AUTO_TEST_CASE( test_mono_join_temporary )
{
    using namespace data;
    using namespace data::monomorphic;
    using value_t = int;
    using op_t = op_identity;
    using ds_t = fake_dataset<op_t, value_t>;
  
    static_assert(boost::unit_test::data::monomorphic::is_dataset<ds_t>::value, "this should be a dataset");
  
    mono_invocation ic;
    data::for_each_sample( data::make( fake_dataset<op_t, value_t>() ) + data::make( fake_dataset<op_t, value_t>() ),
                           ic );
  
    BOOST_TEST(ic.m_value == 4);
  
    using ds2_t = fake_dataset<op_copy, counted_int>;
    mono_check_current_number ic2;
    counted_int::current_nb_instances = 0;
    data::for_each_sample( data::make( ds2_t() ) + data::make( ds2_t() ),
                           ic2 );
  
    // 2 datasets
    // (2 + 3) * 4: 4 iterations, 3 copies each
    BOOST_TEST(ic2.m_value == (2 + 3) * 4);

    using ds2_t = fake_dataset<op_identity, counted_int>;
    mono_check_current_number ic3;
    counted_int::current_nb_instances = 0;
    data::for_each_sample( data::make( ds2_t() ) + data::make( ds2_t() ),
                           ic3 );
  
    // 2 datasets
    // (2 + 3) * 4: 4 iterations, 3 copies each
    BOOST_TEST(ic3.m_value == (2 + 3) * 4);
}

// EOF

