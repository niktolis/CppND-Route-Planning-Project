#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y,
                           float end_x, float end_y)
    : m_Model(model) {
  // Convert inputs to percentage:
  start_x *= 0.01;
  start_y *= 0.01;
  end_x *= 0.01;
  end_y *= 0.01;

  // Store the nodes you find in the RoutePlanner's start_node and end_node
  // attributes.
  this->start_node = &m_Model.FindClosestNode(start_x, start_y);
  this->end_node = &m_Model.FindClosestNode(end_x, end_y);
}

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
  // Using the distance to the end_node to calculate the h value
  return node->distance(*this->end_node);
}

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {

  // Populate the current_node.neighbors vector
  current_node->FindNeighbors();

  // For each node in the neighbors vector set the parent, the h and g value and
  // add it to the open list. Mark the node as visited.
  for (auto node : current_node->neighbors) {
    node->parent = current_node;
    node->g_value = current_node->g_value + node->distance(*current_node);
    node->h_value = this->CalculateHValue(node);
    this->open_list.emplace_back(node);
    node->visited = true;
  }
}

bool RoutePlanner::Compare(const RouteModel::Node *n1,
                           const RouteModel::Node *n2) {
  // As in lesson compare against total g_value and h_value of each node to sort

  float f1 = n1->g_value + n1->h_value;
  float f2 = n2->g_value + n2->h_value;

  return f1 > f2;
}

RouteModel::Node *RoutePlanner::NextNode() {
  // Sort the actual open list based on the fum of h and g value using NodeSort
  NodeSort(&this->open_list);

  // Pick up the node with the lowest cost and remove it from the open list
  auto current_node = open_list.back();
  open_list.pop_back();

  return current_node;
}

std::vector<RouteModel::Node>
RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
  // Create path_found vector
  distance = 0.0f;
  std::vector<RouteModel::Node> path_found;

  // First the current node to the path. This will be the end_node.
  path_found.emplace_back(*current_node);

  // Traversing the nodes back using the parent chain calculate the total
  // distance and add sequentially the nodes to the path_found vector up until
  // the start_node is reached.
  while (current_node != this->start_node) {
    distance += current_node->distance(*current_node->parent);
    current_node = current_node->parent;
    path_found.emplace_back(*current_node);
  }

  // Reverse the vector so first element becomes the start_node and last element
  // becomes the end_node.
  std::reverse(path_found.begin(), path_found.end());
  distance *= m_Model.MetricScale(); // Multiply the distance by the scale of
                                     // the map to get meters.
  return path_found;
}

void RoutePlanner::AStarSearch() {
  RouteModel::Node *current_node = nullptr;

  // initialize the starting node and mark it as visited.
  current_node = this->start_node;
  current_node->visited = true;

  // Add the neighbours of start_node to the open list
  this->AddNeighbors(current_node);

  // For as long the list is not empty and the end_not has not been reached pick
  // up the next node with the less cost and add it's neighbors to the open list
  while (this->open_list.size() > 0) {

    // If the end_node is reached we finished the path sequence so we can
    // construct it and return
    if (current_node == this->end_node) {
      this->m_Model.path = this->ConstructFinalPath(current_node);
      return;
    }
    current_node = this->NextNode();
    this->AddNeighbors(current_node);
  }
}
