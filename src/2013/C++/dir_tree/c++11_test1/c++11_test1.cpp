
#include "stdafx.h"

class FileSystemNode : public boost::enable_shared_from_this<FileSystemNode>
{
public:
	typedef std::vector<boost::shared_ptr<FileSystemNode>> CollectionType;
	typedef boost::shared_ptr<FileSystemNode> ElementType;

protected:
	bool _isDirectory;
	std::wstring _name;
	boost::weak_ptr<FileSystemNode> _parent;
	FileSystemNode::CollectionType _children;
	_ULonglong _size;

public:
	FileSystemNode() :
		_isDirectory(false),
		_size(0UL)
	{
	}
	inline bool IsDirectory() const
	{
		return _isDirectory;
	}
	inline int GetLevel() const
	{
		auto p = _parent.lock();
		return (p == nullptr) ? 0 : p->GetLevel() + 1;
	}
	inline const std::wstring& GetName() const
	{
		return _name;
	}
	inline void SetName(const std::wstring& name)
	{
		_name = name;
	}
	inline std::wstring GetFullName() const
	{
		auto p = _parent.lock();
		return (p == nullptr) ? GetName() : p->GetFullName() + L"\\" + GetName();
	}
	inline const std::vector<ElementType>& GetChildren() const
	{
		return _children;
	}
	inline const ElementType GetParent() const
	{
		return _parent.lock();
	}
	virtual void AddChild(const ElementType& node)
	{
		node->_parent = shared_from_this();
		_children.push_back(node);
	}
	template<typename T> T* Get() const
	{
		return (T*)this;
	}
	inline const _ULonglong GetSize() const
	{
		return _size;
	}
	inline void SetSize(const _ULonglong& size)
	{
		_size = size;
	}
};

class DirectoryNode : public FileSystemNode
{
private:
public:
	DirectoryNode()
	{
		_isDirectory = true;
	}
};

class FileNode : public FileSystemNode
{
private:
public:
	FileNode()
	{
		_isDirectory = false;
	}
};

template<typename T>
class NumberFormatter : public std::numpunct<T>
{
	using std::numpunct<T>::string_type;
	using std::numpunct<T>::char_type;
protected:
	char_type do_thousands_sep() const
	{
		return T(',');
	}
	std::string do_grouping() const
	{
		return std::string("\3");
	}
};

void DumpTree(const FileSystemNode::ElementType node, bool isFlat)
{
	auto children = node->GetChildren();
	int level = node->GetLevel();

	if (isFlat)
	{
		if (node->IsDirectory())
		{
			std::wcout <<
				boost::wformat(L"%4s %10ld %-50s") % L"D" % node->GetSize() % node->GetFullName() <<
				std::endl;
		}
		else
		{
			std::wcout <<
				boost::wformat(L"%4s %10ld %-50s") % L"F" % node->GetSize() % node->GetFullName() <<
				std::endl;
		}
	}
	else
	{
		std::wstring indent(level * 4, ' ');
		if (node->IsDirectory())
		{
			std::wcout << indent << node->GetName() << L"  (" << node->GetSize() << L" bytes)" << std::endl;
		}
		else
		{
			std::wcout << indent << node->GetName() << L"  (" << node->GetSize() << L" bytes)" << std::endl;
		}
	}

	boost::for_each(children, [&](FileSystemNode::ElementType child) -> void {
		DumpTree(child, isFlat);
	});
}

void CreateTree(const FileSystemNode::ElementType& node)
{
	auto path = std::tr2::sys::wpath(node->GetFullName());

	if (std::tr2::sys::is_directory(path))
	{
		std::tr2::sys::wdirectory_iterator begin(path);
		std::tr2::sys::wdirectory_iterator end;
		auto range = boost::make_iterator_range(begin, end);

		std::for_each(range.begin(), range.end(), 
			[&](const std::tr2::sys::wdirectory_entry& ent) -> void {

				auto childPath = std::tr2::sys::wpath(node->GetFullName() + L"/" + ent.path().string());

				if (std::tr2::sys::is_directory(childPath))
				{
					FileSystemNode::ElementType child(new DirectoryNode());
					node->AddChild(child);
					child->SetName(ent.path().filename());

					CreateTree(child);
					child->GetParent()->SetSize(child->GetSize() + child->GetParent()->GetSize());
				}
				else
				{
					FileSystemNode::ElementType child(new FileNode());
					node->AddChild(child);
					child->SetName(ent.path().filename());
					child->SetSize(std::tr2::sys::file_size(childPath));
					child->GetParent()->SetSize(child->GetSize() + child->GetParent()->GetSize());
				}
			}
		);
	}
	else
	{
		FileSystemNode::ElementType child(new FileNode());
		node->AddChild(child);
		child->SetName(path.filename());
		child->SetSize(std::tr2::sys::file_size(path));
		child->GetParent()->SetSize(child->GetSize() + child->GetParent()->GetSize());
	}
}


int wmain(int argc, wchar_t* argv[])
{
	_wsetlocale(0, L"");

	const std::wstring root(L"F:\\DropBox");
	//const std::wstring root(L"C:\\Windows\\System32\\wbem");

	FileSystemNode::ElementType root_node(new DirectoryNode());
	root_node->SetName(root);

	CreateTree(root_node);

	DumpTree(root_node, true);

	std::wcout << std::wstring(100, '-') << std::endl;

	DumpTree(root_node, false);

	//std::locale fmt(std::locale::classic(), new NumberFormatter<wchar_t>());
	//std::wcout.imbue(fmt);
	//std::wcout << 1234567 << std::endl;

	return 0;
}

