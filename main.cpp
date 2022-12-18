#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

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
	int id;
	int amount;
	bool check_live;
	int mission;
	Tile curr;
	Tile prev;
	Tile next;
	Tile dest;
};

void print_info(int width, int height)
{
    cout << "MESSAGE " << width << "," << height;
}

struct field_info
{
	vector<Tile> tiles;
	vector<Tile> my_tiles;
	vector<Tile> opp_tiles;
	vector<Tile> neutral_tiles;
	vector<Tile> opp_units;
	vector<Tile> my_recyclers;
	vector<Tile> opp_recyclers;
	vector<one_unit> my_units;
	vector<one_unit> snap_shot;
};

bool is_coor(Tile& lhs, Tile& rhs)
{
	if (lhs.x == rhs.x && lhs.y == rhs.y)
		return true;
	return false;
}

one_unit save_myUnit(vector<one_unit>& prev_pos, Tile& tile)
{
	static int id;
	for(one_unit unit : prev_pos)
	{
		if (unit.id == 0)
		{
			unit.id = ++id;
			unit.amount = tile.units;
			unit.check_live = true;
			unit.prev = unit.curr;
			unit.curr = tile;
			/* wait
			mission, next, dest
			*/
			prev_pos.emplace_back(unit);
			break;
		}
		else if (is_coor(unit.prev, tile))
		{
			/* wait
			id, check_live, mission, next, dest
			*/
			unit.prev = unit.curr;
			unit.curr = tile;
			unit.amount = tile.units;
			prev_pos.emplace_back(unit);
			break;
		}
	}
	return one_unit();
}

void save_field(field_info& info, Tile& tile)
{
	info.tiles.emplace_back(tile);

	if (tile.owner == ME)
	{
		info.my_tiles.emplace_back(tile);
		if (tile.units > 0)
		{
			one_unit my_unit;
			my_unit.curr = tile;
			my_unit.amount = tile.units;
			info.my_units.emplace_back(my_unit);
			// one_unit my_unit = save_myUnit(info.prev_pos, tile);
			// info.prev_pos.emplace_back(my_unit);
		}
		else if (tile.recycler)
		{
			info.my_recyclers.emplace_back(tile);
		}
	}
	else if (tile.owner == OPP)
	{
		info.opp_tiles.emplace_back(tile);
		if (tile.units > 0)
		{
			info.opp_units.emplace_back(tile);
		}
		else if (tile.recycler)
		{
			info.opp_recyclers.emplace_back(tile);
		}
	} else
	{
		info.neutral_tiles.emplace_back(tile);
	}
}

void cmd_move(field_info& info, vector<string>& actions)
{
	static int id;
	for (one_unit curr_unit : info.my_units)
	{
		int prev_units_size = info.snap_shot.size() - 1;
		for (int i = 0; i <= prev_units_size; ++i)
		{
			one_unit prev_unit = info.snap_shot[i];
			if (is_coor(curr_unit.curr, prev_unit.next))
			{
				curr_unit.id = prev_unit.id;
				curr_unit.check_live = prev_unit.check_live;
				curr_unit.mission = prev_unit.mission;
				curr_unit.dest = prev_unit.dest;
				/* wait
				next
				*/
				info.snap_shot.erase(info.snap_shot.begin() + i);
				break;
			}
			else
			{
				curr_unit.id = ++id;
				curr_unit.check_live = true;
				/* wait
				next, dest, misson
				*/
				break;
			}
		}
		//
		bool should_move = false; // TODO: pick whether to move units from here

		if (should_move)
		{
			int amount = 0; // TODO: pick amount of units to move
			Tile target; // TODO: pick a destination
			ostringstream action;
				action << "MOVE " << amount << " " << curr_unit.curr << " " << target;
				actions.emplace_back(action.str());
		}
	}
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

void set_info(field_info& info)
{
	info.tiles.clear();
	info.my_tiles.clear();
	info.opp_tiles.clear();
	info.neutral_tiles.clear();
	info.opp_units.clear();
	info.my_recyclers.clear();
	info.opp_recyclers.clear();
	info.my_units.clear();
}

int main()
{
    int width;
    int height;
    cin >> width >> height; cin.ignore();

	field_info info;
	print_info(width, height);

    // game loop
    while (1) {
		set_info(info);
		info.tiles.reserve(width * height);
        int my_matter;
        int opp_matter;
        cin >> my_matter >> opp_matter; cin.ignore();
        for (int x = 0; x < height; x++)
		{
            for (int y = 0; y < width; y++)
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
				save_field(info, tile);

            }
        }

        vector<string> actions;
		cmd_move(info, actions);
		end_cmd(actions);
    }
}
