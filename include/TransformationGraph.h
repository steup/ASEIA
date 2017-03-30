#pragma once

#include <Transformation.h>
#include <CompositeTransformation.h>

#include <vector>
#include <iterator>

class TransformationGraph {
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
};
