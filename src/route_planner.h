#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include "route_model.h"
#include <iostream>
#include <string>
#include <vector>

class RoutePlanner {
public:
  RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x,
               float end_y);
  // Add public variables or methods declarations here.
  float GetDistance() const { return distance; }
  void AStarSearch();

  // The following methods have been made public so we can test them
  // individually.
  void AddNeighbors(RouteModel::Node *current_node);
  float CalculateHValue(RouteModel::Node const *node);
  std::vector<RouteModel::Node> ConstructFinalPath(RouteModel::Node *);
  RouteModel::Node *NextNode();

private:
  // Add private variables or methods declarations here.
  std::vector<RouteModel::Node *> open_list;
  RouteModel::Node *start_node;
  RouteModel::Node *end_node;

  float distance = 0.0f;
  RouteModel &m_Model;

  bool Compare(const RouteModel::Node *, const RouteModel::Node *);

  void NodeSort(std::vector<RouteModel::Node *> *v) {
    // Using lambda for comparing on sort with a non-static member function
    // implemented (Compare). A detailed explanation on this approach -with
    // metrics- can be found here: https://stackoverflow.com/a/24574712/22335122

    std::sort(v->begin(), v->end(),
              [this](auto n1, auto n2) { return Compare(n1, n2); });
  }
};

#endif
