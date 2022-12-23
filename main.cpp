#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdarg.h>

using namespace std;

vector<string> g_actions;

static constexpr int ME = 1;
static constexpr int OPP = 0;
static constexpr int NONE = -1;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

struct Tile
{
    int x, y;
	int scrap_amount, owner, units;
    bool recycler, can_build, can_spawn, in_range_of_recycler;
    ostream& dump(ostream& ioOut) const
	{
        ioOut << x << " " << y;
        return ioOut;
    }
};
ostream& operator<<(ostream& ioOut, const Tile& obj) { return obj.dump(ioOut); }

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
	bool should_move;
};

struct strategy_info
{
	int my_side;
	int opp_side;
	Tile dest;
};

struct field_info
{
	int width_field;
	int height_field;
	vector<vector<Tile>>	tiles;
	vector<Tile>	my_tiles;
	vector<Tile>	opp_tiles;
	vector<Tile>	neutral_tiles;
	vector<Tile>	opp_units;
	vector<Tile>	my_recyclers;
	vector<Tile> opp_recyclers;
	vector<one_unit> my_units;
	vector<one_unit> snap_shot;
	Tile my_camp;
	Tile opp_camp;
	int turn;
};

void set_info(field_info& f_info, vector<string>& actions)
{
	for (int i = 0; i < f_info.tiles.size(); ++i)
		f_info.tiles[i].clear();
	f_info.tiles.resize(f_info.height_field);
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

void check_camp(Tile& camp, Tile& tile)
{
	if (camp.x == -1 && tile.can_build)
	{
		camp = tile;
	}
}

void save_field(field_info& f_info, Tile& tile, one_unit& my_unit)
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
			f_info.opp_units.emplace_back(tile);
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

void set_side(field_info& f_info, strategy_info& s_info)
{
	if (f_info.my_camp.x - f_info.opp_camp.x > 0)
	{

	}
	else
	{
	}
}

void set_strategy(field_info& f_info, strategy_info& s_info)
{
	set_side(f_info, s_info);
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

int main()
{
    int width;
    int height;
    cin >> width >> height; cin.ignore();

	field_info f_info;
	strategy_info s_info;
	f_info.width_field = width;
	f_info.height_field = height;
	f_info.tiles.reserve(height);
	f_info.my_camp = {-1, -1, 0, 0, 0, 0, 0, 0, 0};
	f_info.opp_camp = {-1, -1, 0, 0, 0, 0, 0, 0, 0};

    // game loop
    while (1)
	{
		print_info(2, f_info.width_field, f_info.height_field);
        int my_matter;
        int opp_matter;
        cin >> my_matter >> opp_matter; cin.ignore();
		for (int y = 0; y < height; y++)
		{
			f_info.tiles[y].reserve(width);
			for (int x = 0; x < width; x++)
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
				f_info.tiles[y].emplace_back(tile);
				one_unit my_unit;
				save_field(f_info, tile, my_unit);
            }
        }
		vector<string>& actions = g_actions;
		set_strategy(f_info, s_info);
		ctrl_units(f_info, actions);
		end_cmd(actions);
		set_info(f_info, actions);
    }
}
