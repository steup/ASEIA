#pragma once

#include <Transformation.h>
#include <CompositeTransformation.h>

#include <boost/graph/adjacency_list.hpp>

#include <vector>
#include <iterator>
#include <iosfwd>

class TransformationGraph {
  private:
    boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
                                        TransformationPtr, EventID> mGraph;
  public:
    using OutIt = std::back_insert_iterator<std::vector<CompositeTransformation>>;
    using EventIDs = Transformation::EventIDs;

    /** \brief Insert heterogeneus Transformation into transformation graph
     *  \param trans TransformationPtr to Transformation to insert
     **/
     void insert(TransformationPtr tPtr);

    /** \brief Remove heterogeneus Transformation into transformation graph
     *  \param trans TransformationPtr to Transformation to remove
     **/
     void remove(TransformationPtr tPtr);

     /** \brief generate all possible CompositeTransformations
      *  \param goal the target EventType
      *  \param ids the available EventIds published in the system
      *  \param it OutputIterator to store resulting CompositeTransformations
      *  This method uses the registered heterogeneus Transformations to deduce CompositeTransformations
      **/
     void generate(const EventType& type, const EventIDs& ids, OutIt it) const;

     /** \brief clear graph and remove all Transformations and connections **/
     void clear();

   friend std::ostream& operator<<(std::ostream&, const TransformationGraph&);
};

std::ostream& operator<<(std::ostream& o, const TransformationGraph& g);
