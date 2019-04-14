
interface Action extends Delegate{
}
interface Action0 extends Action{
	void Invoke();
}
interface Action1<T> extends Action{
	void Invoke(T arg);
}
interface Action2<T1, T2> extends Action{
	void Invoke(T1 arg1, T2 arg2);
}