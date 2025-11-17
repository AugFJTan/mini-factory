#include "FileUtil.h"

void parse_animation_file(std::string filename, TileID start_tile_id, Spritesheet* spritesheet, std::map<std::string, TileID>& tile_lookup,
	sPtr<AnimationFrames>& animation_frames, std::vector<uPtr<Animation>>& animations) {
	std::ifstream file;
	file.open(filename);

	std::string data;
	int lines_read = 0;
	int frames, fps;
	std::string loop;
	std::string animation_class;
	std::string particle_effect_id;
	ParticleEffect particle_effect;
	std::string tile_id;
	int x, y, width, height;
	int x_offset, y_offset;
	int n = start_tile_id;

	while (std::getline(file, data)) {
		if (data == "")
			continue;
		if (data[0] == '#')
			continue;
		std::istringstream ss(data);
		switch(lines_read) {
			case 0:
				ss >> frames >> fps >> loop;
				animation_frames = std::make_shared<AnimationFrames>(frames, fps, loop == "True");
				std::cout << "Frames = " << frames << ", FPS = " << fps << ", Loop = " << loop << std::endl;
				break;
			case 1:
				ss >> width >> height;
				std::cout << "Width = " << width << ", Height = " << height << std::endl;
				break;
			case 2:
				ss >> animation_class;
				std::cout << "Class = " << animation_class << std::endl;
				break;
			case 3:
				ss >> particle_effect_id;
				std::cout << "Particle effect = " << particle_effect_id << std::endl;
				if (particle_effect_id != "None")
					particle_effect = parse_particle_effect_file(particle_effect_id);
				break;
			default:
				if (animation_class == "Animation") {
					ss >> tile_id >> x >> y;
					SDL_Rect rect = {x, y, width, height};
					animations.push_back(std::make_unique<Animation>(spritesheet, animation_frames, rect));
				} else if (animation_class == "MachineAnimation") {
					ss >> tile_id >> x >> y >> x_offset >> y_offset;
					SDL_Rect rect = {x, y, width, height};
					SDL_Point offset = {x_offset, y_offset};
					animations.push_back(std::make_unique<MachineAnimation>(spritesheet, animation_frames, rect, offset, particle_effect));
				}
				tile_lookup[tile_id] = static_cast<TileID>(n++);
				std::cout << tile_id << " @ (" << x << ", " << y << ")" << std::endl;
				break;
		}
		lines_read++;
	}
}

ParticleEffect parse_particle_effect_file(std::string particle_effect_id) {
	std::string filename = "../config/" + particle_effect_id + ".txt";

	std::ifstream file;
	file.open(filename);

	std::string data;
	int lines_read = 0;
	int start_frame, end_frame;
	int width, height;
	int x, y, x_offset, y_offset;
	ParticleEffect particle_effect;

	while (std::getline(file, data)) {
		if (data == "")
			continue;
		if (data[0] == '#')
			continue;
		std::istringstream ss(data);
		switch(lines_read) {
			case 0:
				ss >> start_frame >> end_frame;
				std::cout << "Start = " << start_frame << ", End = " << end_frame << std::endl;
				break;
			case 1:
				ss >> width >> height;
				std::cout << "Width = " << width << ", Height = " << height << std::endl;
				break;
			default:
				ss >> x >> y >> x_offset >> y_offset;
				std::cout << "(" << x << ", " << y << "), (" << x_offset << ", " << y_offset << ")" << std::endl;
				break;
		}
		lines_read++;
	}

	particle_effect.start_frame = start_frame;
	particle_effect.end_frame = end_frame;
	particle_effect.rect = {x, y, width, height};
	particle_effect.offset = {x_offset, y_offset};

	return particle_effect;
}

void parse_layout_file(Tile map[], std::map<std::string, TileID>& tile_lookup, Path& path) {
	std::ifstream file;
	file.open("../config/layout.txt");

	std::string data;
	std::string tile_id;
	int x, y;

	while (std::getline(file, data)) {
		if (data == "")
			continue;
		std::istringstream ss(data);
		ss >> tile_id >> x >> y;
		SDL_Point pos = {x, y};
		Tile belt(BELT, tile_lookup[tile_id]);
		map[to_index(pos)] = belt;
		std::cout << tile_id << " " << "(" << x << ", " << y << ")" << std::endl;
	}
}

void parse_item_lanes_file(std::vector<std::vector<SDL_Point>>& belt_lane_offsets) {
	std::ifstream file;
	file.open("../config/item_lanes.txt");

	std::string data;
	std::string lane_id, points;

	while (std::getline(file, data)) {
		std::istringstream ss(data);
		ss >> lane_id >> points;
		std::cout << "Lane: " << lane_id << " Pos: " << points << std::endl;
		size_t idx = 0, lbracket = 0, rbracket = 0;
		std::vector<SDL_Point> offsets;
		while(idx < points.length()) {
			lbracket = points.find('(', idx);
			rbracket = points.find(')', idx);
			std::string coord = points.substr(lbracket+1, rbracket-lbracket-1);
			size_t comma = coord.find(',');
			SDL_Point pos;
			pos.x = std::stoi(coord.substr(0, comma));
			pos.y = std::stoi(coord.substr(comma+1));
			offsets.push_back(pos);
			idx = rbracket + 2;
		}
		belt_lane_offsets.push_back(offsets);
	}
}
