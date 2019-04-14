
class Stopwatch{
	private long _start = 0L;
	private long _stop = 0L;
	private long _result = 0L;

	public Stopwatch(){
	}

	public void Start(){
		_start = System.currentTimeMillis();
	}

	public void Stop(){
		_stop = System.currentTimeMillis();
		_result = _stop - _start;
	}

	public long getMilliSeconds(){
		return _result;
	}
}
