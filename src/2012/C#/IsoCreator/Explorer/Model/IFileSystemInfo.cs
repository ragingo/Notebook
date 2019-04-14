
namespace Explorer.Model
{
	interface IFileSystemInfo
	{
		bool IsDrive { get; }
		bool IsDirectory { get; }
		bool IsFile { get; }
		string Identity { get; }
		TResult Get<TResult>() where TResult : class;
	}
}
