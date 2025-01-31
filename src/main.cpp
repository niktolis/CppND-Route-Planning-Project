#include "render.h"
#include "route_model.h"
#include "route_planner.h"
#include <fstream>
#include <io2d.h>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

// Project Rubric Defined limits for the coordinates
#define MIN_COORD_VALUE 0.0f
#define MAX_COORD_VALUE 100.0f

using namespace std::experimental;

static std::optional<std::vector<std::byte>> ReadFile(const std::string &path) {
  std::ifstream is{path, std::ios::binary | std::ios::ate};
  if (!is)
    return std::nullopt;

  auto size = is.tellg();
  std::vector<std::byte> contents(size);

  is.seekg(0);
  is.read((char *)contents.data(), size);

  if (contents.empty())
    return std::nullopt;
  return std::move(contents);
}

static std::vector<float> ParseLine(const std::string &line) {
  std::istringstream sline(line, ',');
  float x, y;
  char c;

  std::vector<float> row;

  while (sline >> x >> c >> y && c == ',') {
    if ((MIN_COORD_VALUE <= x && x <= MAX_COORD_VALUE) &&
        (MIN_COORD_VALUE <= y && y <= MAX_COORD_VALUE)) {
      row.push_back(x);
      row.push_back(y);
    } else {
      std::cerr << "Error: The coordinates are out of the defined limits!\n";
    }
  }
  assert(row.size() == 2);
  return row;
}
int main(int argc, const char **argv) {
  std::string osm_data_file = "";
  if (argc > 1) {
    for (int i = 1; i < argc; ++i)
      if (std::string_view{argv[i]} == "-f" && ++i < argc)
        osm_data_file = argv[i];
  } else {
    std::cout << "To specify a map file use the following format: "
              << std::endl;
    std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;
    osm_data_file = "../map.osm"; // Change due to build config
  }

  std::vector<std::byte> osm_data;

  if (osm_data.empty() && !osm_data_file.empty()) {
    std::cout << "Reading OpenStreetMap data from the following file: "
              << osm_data_file << std::endl;
    auto data = ReadFile(osm_data_file);
    if (!data)
      std::cout << "Failed to read." << std::endl;
    else
      osm_data = std::move(*data);
  }

  // User Input Interface for start and end points on the map
  //
  std::string start_input, end_input;

  std::cout << "Give start point coordinates x,y. [0<=x,y<=100](e.g. "
               "1.0749,35.4090)\n";
  getline(std::cin, start_input);
  std::vector<float> start = ParseLine(start_input);
  std::cout << "Give end point coordinates x,y. [0<=x,y<=100](e.g. "
               "1.0749,35.43333)\n";
  getline(std::cin, end_input);
  std::vector<float> end = ParseLine(end_input);

  // Build Model.
  RouteModel model{osm_data};

  // Create RoutePlanner object and perform A* search.
  RoutePlanner route_planner{model, start[0], start[1], end[0], end[1]};
  route_planner.AStarSearch();

  std::cout << "Distance: " << route_planner.GetDistance() << " meters. \n";

  // Render results of search.
  Render render{model};

  auto display = io2d::output_surface{400,
                                      400,
                                      io2d::format::argb32,
                                      io2d::scaling::none,
                                      io2d::refresh_style::fixed,
                                      30};
  display.size_change_callback([](io2d::output_surface &surface) {
    surface.dimensions(surface.display_dimensions());
  });
  display.draw_callback(
      [&](io2d::output_surface &surface) { render.Display(surface); });
  display.begin_show();
}
