
#include "stdafx.h"

class tree_node;

typedef std::map<DWORD, boost::shared_ptr<tree_node>> tree_map;
typedef std::pair<DWORD, boost::shared_ptr<tree_node>> tree_map_element;

class tree_node
{
public:
	Process::ProcessElement value;
	boost::shared_ptr<tree_map> children;
	bool is_root;
};

const tree_map create_process_tree(const Process::ProcessContainer processes)
{
	tree_map map;

	for each(auto process in processes)
	{
		if (process->GetId() == 0UL)
		{
			continue;
		}
		boost::shared_ptr<tree_node> node(new tree_node);
		node->value = process;
		node->is_root = true;
		node->children = boost::shared_ptr<tree_map>(new tree_map());
		map.insert(std::make_pair(process->GetId(), node));
	}

	for each(auto process in processes)
	{
		auto predParent = [&](tree_map_element src) {
			return src.first == process->GetParentId();
		};
		auto predSelf = [&](tree_map_element src) {
			return src.first == process->GetId();
		};

		tree_map::iterator parent;
		if ((parent = std::find_if(map.begin(), map.end(), predParent)) != map.end())
		{
			boost::shared_ptr<tree_node> node(new tree_node);
			node->value = process;
			node->is_root = false;
			node->children = boost::shared_ptr<tree_map>(new tree_map());
			parent->second->children->insert(std::make_pair(process->GetId(), node));

			tree_map::iterator self;
			if ((self = std::find_if(map.begin(), map.end(), predSelf)) != map.end())
			{
				self->second->is_root = false;
			}
		}
	}

	return map;
}

void dump_process_tree_core(const tree_map &tree, const tree_map_element &node, int depth)
{
	tstring name;
	for(int i=0; i<depth; i++)
	{
		name.append(_T("    "));
	}
	name.append(node.second->value->GetName());
	tcout
		<< tformat(_T("%-20s (%5d) (ŠK‘w : %2d, e : %5d)"))
			% name
			% node.second->value->GetId()
			% depth
			% node.second->value->GetParentId()
		<< std::endl;

	for each(auto childNode in *node.second->children)
	{
		dump_process_tree_core(tree, childNode, depth+1);
	}
}

void dump_process_tree(const tree_map &tree)
{
	int depth = 0;
	for each(auto node in tree)
	{
		if (node.second->is_root)
		{
			// debug
			if (node.second->value->GetName() == _T("wininit.exe"))
			{
				int i=0;
			}
			dump_process_tree_core(tree, node, depth);
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_tsetlocale(LC_ALL, _T(""));

	auto processes = Process::GetProcessesByName(_T("^.*$"));
	boost::sort(processes, 
		[](const Process::ProcessElement &lhs, const Process::ProcessElement &rhs) {
			return lhs->GetId() < rhs->GetId();
		}
	);

	for each(auto process in processes)
	{
		tcout 
			<< tformat(_T("%-10d %-30s %-10d"))
				% process->GetId()
				% process->GetName()
				% process->GetParentId()
			<< std::endl;
	}

	auto tree = create_process_tree(processes);
	dump_process_tree(tree);

	return 0;
}

