#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdarg.h>
#include <utility>

using namespace std;

vector<string> g_actions;

static constexpr int ME = 1;
static constexpr int OPP = 0;
static constexpr int NONE = -1;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

/* define boder_type */
#define MY_AREA 1
#define OPP_AREA 2
#define MY_LIMIT_AREA 3
#define OPP_LIMIT_AREA 4
#define NEUTRAL_AREA 5

/* define symmetry_type */
#define VERTICAL 1
#define MIRROR 2

/* define x_side */
#define L_SIDE 1
#define R_SIDE 2

/* define y_side */
#define SAME 0
#define U_SIDE 1
#define D_SIDE 2

struct Tile_around;
struct Tile
{
    int x, y;
	int scrap_amount;
	int owner;
	int units;
	int border_type;
    bool recycler;
	bool can_build;
	bool can_spawn;
	bool in_range_of_recycler;
	int footprint;
	Tile_around *around;
    ostream& dump(ostream& ioOut) const
	{
        ioOut << x << " " << y;
        return ioOut;
    }
	bool operator== (const Tile& obj) const
	{
		return (x == obj.x && y == obj.y);
	}
};
ostream& operator<<(ostream& ioOut, const Tile& obj) { return obj.dump(ioOut); }

struct Tile_around
{
	Tile up;
	Tile down;
	Tile left;
	Tile right;

	Tile up_left;
	Tile up_right;
	Tile down_left;
	Tile down_right;
};

class one_unit
{
public:
	Tile curr;
	int amount;
	int id;
	bool check_live;
	int mission;
	Tile dest;
	Tile prev;
	Tile next;
	pair<Tile, Tile> mark_target;
	bool should_move;
};

struct strategy_info
{
	int turn;
	bool check_neutral_line;

	int my_side_x;
	int my_side_y;
	int my_tiles_count;
	int my_scrap;
	vector<Tile>	my_limit_line;

	int opp_side_x;
	int opp_side_y;
	int opp_tiles_count;
	int opp_scrap;
	vector<Tile>	opp_limit_line;

	int neutral_tiles_count;
	vector<Tile>	neutral_line;
};

struct field_info
{
	int symmetry_type;
	int width_field;
	int height_field;
	vector<vector<Tile>>	tiles;
	vector<Tile>	my_tiles;
	vector<Tile>	opp_tiles;
	vector<Tile>	neutral_tiles;
	vector<one_unit>	opp_units;
	vector<Tile>	my_recyclers;
	vector<Tile>	opp_recyclers;
	vector<one_unit> my_units;
	vector<one_unit> snap_shot;
	Tile my_camp;
	Tile opp_camp;
};

void print_info(int num_args, ...)
{
	va_list args;
	va_start(args, num_args);
	ostringstream action;

	action << "MESSAGE ";
	for (int i = 0; i < num_args; ++i)
	{
		action << va_arg(args, int) << " ";
	}
	g_actions.emplace_back(action.str());
	va_end(args);
}

void set_info(field_info& f_info, vector<string>& actions)
{
	for (int i = 0; i < f_info.tiles.size(); ++i)
		f_info.tiles[i].clear();
	f_info.tiles.resize(f_info.width_field);
	f_info.my_tiles.clear();
	f_info.opp_tiles.clear();
	f_info.neutral_tiles.clear();
	f_info.opp_units.clear();
	f_info.my_recyclers.clear();
	f_info.opp_recyclers.clear();
	f_info.my_units.clear();
	if(f_info.my_camp.scrap_amount == 1 && f_info.my_camp.in_range_of_recycler == 1)
	{
		f_info.my_camp.x = -1;
		f_info.my_camp.y = -1;
	}
	if(f_info.opp_camp.scrap_amount == 1 && f_info.opp_camp.in_range_of_recycler == 1)
	{
		f_info.opp_camp.x = -1;
		f_info.opp_camp.y = -1;
	}
	actions.clear();
}

void end_cmd(vector<string>& actions)
{
	if (actions.empty())
	{
			cout << "WAIT" << endl;
	}
	else
	{
		for (vector<string>::iterator it = actions.begin(); it != actions.end(); ++it)
		{
			cout << *it << ";";
		}
		cout << endl;
	}
}

void check_camp(Tile& camp, Tile& tile)
{
	if (camp.x == -1 && tile.units == 0)
	{
		camp = tile;
	}
}

void save_field(field_info& f_info, Tile& tile, one_unit& my_unit, one_unit& opp_unit)
{
	if (tile.owner == ME)
	{
		f_info.my_tiles.emplace_back(tile);
		if (tile.units > 0)
		{
			my_unit.curr = tile;
			my_unit.amount = tile.units;
			f_info.my_units.emplace_back(my_unit);
		}
		else if (tile.recycler)
		{
			f_info.my_recyclers.emplace_back(tile);
		}
		check_camp(f_info.my_camp, tile);
	}
	else if (tile.owner == OPP)
	{
		f_info.opp_tiles.emplace_back(tile);
		if (tile.units > 0)
		{
			opp_unit.curr = tile;
			opp_unit.amount = tile.units;
			f_info.opp_units.emplace_back(opp_unit);
		}
		else if (tile.recycler)
		{
			f_info.opp_recyclers.emplace_back(tile);
		}
		check_camp(f_info.opp_camp, tile);
	}
	else
	{
		f_info.neutral_tiles.emplace_back(tile);
	}
}

void give_mission(field_info& f_info, one_unit& curr_unit)
{

}

void find_route(field_info& f_info, strategy_info& s_info,Tile& start, Tile& end)
{
	if (start == end)
		return;
	if (s_info.my_side_x == L_SIDE)
	{
		find_route(f_info, s_info, start, end);
	}
}

void check_route(field_info& f_info, strategy_info& s_info)
{
	for (int i = 0; i < f_info.my_units.size(); ++i)
	{
		find_route(f_info, s_info, f_info.my_units[i].mark_target.first, f_info.my_units[i].mark_target.second);
		find_route(f_info, s_info, f_info.opp_units[i].mark_target.first, f_info.opp_units[i].mark_target.second);
	}
}

void set_side_y(field_info& f_info, strategy_info& s_info)
{
	s_info.my_side_y = SAME;
	s_info.opp_side_y = SAME;
	if (f_info.my_camp.y > f_info.opp_camp.y)
	{
		s_info.my_side_y = D_SIDE;
		s_info.opp_side_y = U_SIDE;
	}
	else if (f_info.my_camp.y < f_info.opp_camp.y)
	{
		s_info.my_side_y = U_SIDE;
		s_info.opp_side_y = D_SIDE;
	}
}

void set_side_x(field_info& f_info, strategy_info& s_info)
{
	if (f_info.my_camp.x > f_info.opp_camp.x)
	{
		s_info.my_side_x = R_SIDE;
		s_info.opp_side_x = L_SIDE;
	}
	else
	{
		s_info.my_side_x = L_SIDE;
		s_info.opp_side_x = R_SIDE;
	}
}

void set_mark_target(one_unit& unit, Tile& target)
{
	unit.mark_target.first = unit.curr;
	unit.mark_target.second = target;
}

void set_mark_target(field_info& f_info, strategy_info& s_info)
{
	set_mark_target(f_info.my_units[0], f_info.opp_units[3].curr);
	set_mark_target(f_info.my_units[3], f_info.opp_units[0].curr);
	set_mark_target(f_info.opp_units[0], f_info.my_units[3].curr);
	set_mark_target(f_info.opp_units[3], f_info.my_units[0].curr);

	if (f_info.symmetry_type == VERTICAL)
	{
		set_mark_target(f_info.my_units[1], f_info.opp_units[1].curr);
		set_mark_target(f_info.my_units[2], f_info.opp_units[2].curr);
		set_mark_target(f_info.opp_units[1], f_info.my_units[1].curr);
		set_mark_target(f_info.opp_units[2], f_info.my_units[2].curr);
	}
	else
	{
		set_mark_target(f_info.my_units[1], f_info.opp_units[2].curr);
		set_mark_target(f_info.my_units[2], f_info.opp_units[1].curr);
		set_mark_target(f_info.opp_units[1], f_info.my_units[2].curr);
		set_mark_target(f_info.opp_units[2], f_info.my_units[1].curr);
	}
}

void check_symmetry(field_info& f_info, strategy_info& s_info)
{
	set_side_x(f_info, s_info);
	set_side_y(f_info, s_info);
	if (f_info.my_camp.y == f_info.opp_camp.y)
		f_info.symmetry_type = VERTICAL;
	else
		f_info.symmetry_type = MIRROR;
}

void save_border(field_info& f_info, strategy_info& s_info)
{
	int w = f_info.width_field / 2;
	if (s_info.check_neutral_line == true)
	{
		for (int h = 0; h < f_info.height_field; ++h)
		{
			s_info.my_limit_line.emplace_back(f_info.tiles[w - 1][h]);
			s_info.neutral_line.emplace_back(f_info.tiles[w][h]);
			s_info.opp_limit_line.emplace_back(f_info.tiles[w + 1][h]);
		}
	}
	else
	{
		for (int h = 0; h < f_info.height_field; ++h)
		{
			s_info.my_limit_line.emplace_back(f_info.tiles[w - 1][h]);
 			s_info.opp_limit_line.emplace_back(f_info.tiles[w][h]);
		}
	}
}

void set_strategy(field_info& f_info, strategy_info& s_info)
{
	save_border(f_info, s_info);
	check_symmetry(f_info, s_info);
	set_mark_target(f_info, s_info);
	// check_route(f_info, s_info);

}

bool is_coor(Tile& lhs, Tile& rhs)
{
	if (lhs.x == rhs.x && lhs.y == rhs.y)
		return true;
	return false;
}

void set_units(field_info& f_info, one_unit& curr_unit)
{
	static int id;
	int prev_units_size = f_info.snap_shot.size() - 1;
	one_unit prev_unit;
	for (int i = 0; i <= prev_units_size; ++i)
	{
		one_unit prev_unit = f_info.snap_shot[i];
		if (is_coor(curr_unit.curr, prev_unit.next))
		{
			curr_unit.id = prev_unit.id;
			curr_unit.check_live = prev_unit.check_live;
			curr_unit.mission = prev_unit.mission;
			curr_unit.dest = prev_unit.dest;
			/* wait
			next
			*/
			// f_info.snap_shot.erase(f_info.snap_shot.begin() + i);
			return ;
		}
		else
		{
			curr_unit.id = ++id;
			curr_unit.check_live = true;
			curr_unit.dest = f_info.opp_tiles[f_info.opp_tiles.size() - 1];
			/* wait
			next, dest, misson
			*/
			return ;
		}
	}
}

void ctrl_units(field_info& f_info, vector<string>& actions)
{
	for (int i = 0 ; i < f_info.my_units.size(); ++i)
	{
		one_unit& curr_unit = f_info.my_units[i];
		curr_unit.should_move = false; // TODO: pick whether to move units from here
		set_units(f_info, curr_unit);
		give_mission(f_info, curr_unit);
		if (curr_unit.should_move)
		{
			int amount = 0; // TODO: pick amount of units to move
			Tile target; // TODO: pick a destination
			ostringstream action;
				action << "MOVE " << curr_unit.amount << " " << curr_unit.curr << " " << curr_unit.dest;
				actions.emplace_back(action.str());
		}
	}
	f_info.snap_shot.swap(f_info.my_units);
}

void set_around(field_info& f_info, Tile_around* around, Tile& tile, int x, int y)
{
	// around->up = f_info.tiles[x][y - 1];
	// around->down = f_info.tiles[x][y + 1];
	// around->left = f_info.tiles[x - 1][y];
	// around->right = f_info.tiles[x + 1][y];
	// around->up_left = f_info.tiles[x - 1][y - 1];
	// around->up_right = f_info.tiles[x + 1][y - 1];
	// around->down_left = f_info.tiles[x - 1][y + 1];
	// around->down_right = f_info.tiles[x + 1][y + 1];
}

void game_loop(field_info& f_info, strategy_info& s_info,vector<string>& actions)
{
	int my_matter;
	int opp_matter;
	cin >> my_matter >> opp_matter; cin.ignore();
	set_info(f_info, actions);
	int width = f_info.width_field;
	int height = f_info.height_field;
	for (int x = 0; x < width; x++)
	{
		f_info.tiles[x].reserve(height);
		for (int y = 0; y < height; y++)
		{
			int scrap_amount;
			int owner; // 1 = me, 0 = foe, -1 = neutral
			int units;
			int recycler;
			int can_build;
			int can_spawn;
			int in_range_of_recycler;
			cin >> scrap_amount >> owner >> units >> recycler >> can_build >> can_spawn >> in_range_of_recycler; cin.ignore();
			Tile tile = {x, y, scrap_amount, owner, units, recycler == 1, can_build == 1, can_spawn == 1, in_range_of_recycler == 1};
			tile.around = new Tile_around;
			set_around(f_info, tile.around, tile, x, y);
			f_info.tiles[x].emplace_back(tile);
			one_unit my_unit;
			one_unit opp_unit;
			my_unit.dest.x = -1;
			save_field(f_info, tile, my_unit, opp_unit);
		}
	}
}

void middle(field_info& f_info, strategy_info& s_info,vector<string>& actions)
{
    while (1)
	{
		game_loop(f_info, s_info, actions);
		// set_strategy(f_info, s_info);
		ctrl_units(f_info, actions);
		end_cmd(actions);
    }
}

// 최단 경로 구한 후 한계선, 중립선 구하기.
void opening(field_info& f_info, strategy_info& s_info, vector<string>& actions)
{
	game_loop(f_info, s_info, actions);
	set_strategy(f_info, s_info);
	end_cmd(actions);
}

void ending(field_info& f_info, strategy_info& s_info, vector<string>& actions)
{
	end_cmd(actions);
}

int main()
{
    int width;
    int height;
    cin >> width >> height; cin.ignore();

	field_info f_info;
	strategy_info s_info;
	vector<string>& actions = g_actions;
	f_info.width_field = width;
	f_info.height_field = height;
	f_info.tiles.reserve(height);
	f_info.my_camp = {-1, -1, 0, 0, 0, 0, 0, 0, 0, 0};
	f_info.opp_camp = {-1, -1, 0, 0, 0, 0, 0, 0, 0, 0};
	if (width % 2 == 1)
		s_info.check_neutral_line = true;

	opening(f_info, s_info, actions);
	middle(f_info, s_info, actions);
	ending(f_info, s_info, actions);
}
