
interface Func extends Delegate{	
}
interface Func0<TResult> extends Func{
	TResult Invoke();
}
interface Func1<T, TResult> extends Func{
	TResult Invoke(T arg);
}
interface Func2<T1, T2, TResult> extends Func{
	TResult Invoke(T1 arg1, T2 arg2);
}