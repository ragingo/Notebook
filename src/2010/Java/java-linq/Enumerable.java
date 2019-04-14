import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;

final class Enumerable<T> implements Iterable<T>
{
	private ArrayList<T> _source;

	// コンストラクタ
	public Enumerable(T[] source){
		this(Arrays.asList(source));
	}

	// コンストラクタ
	public Enumerable(Collection<T> source){
		_source = new ArrayList<T>(source);
	}

	// シーケンスの全ての要素が条件を満たしているかどうかを判断
	public boolean All(Func1<T, Boolean> predicate){
		for(T value : _source){
			if(!predicate.Invoke(value)){
				return false;
			}
		}
		return true;
	}

	// シーケンスに要素が含まれているかどうかを判断
	public boolean Any(Func1<T, Boolean> predicate){
		for(T value : _source){
			if(predicate.Invoke(value)){
				return true;
			}
		}
		return false;
	}

	// シーケンスの要素を、指定した型に変換
	@SuppressWarnings("unchecked")
	public<TResult> Enumerable<TResult> Cast(Class<TResult> type){
		List<TResult> result = new ArrayList<TResult>(_source.size());

		for(T value : _source){
			if(value instanceof Number){
				Number n = (Number)value;

				if(type == Integer.class){
					result.add((TResult)(Object)n.intValue());
				}
				else if(type == Long.class){
					result.add((TResult)(Object)n.longValue());
				}
				else if(type == Float.class){
					result.add((TResult)(Object)n.floatValue());
				}
				else if(type == Double.class){
					result.add((TResult)(Object)n.doubleValue());
				}
			}
		}
		return new Enumerable<TResult>(result);
	}

	// 2つのシーケンスを連結
	public Enumerable<T> Concat(List<T> target){
		List<T> result = new ArrayList<T>(_source.size() + target.size());
		result.addAll(_source);
		result.addAll(target);
		return new Enumerable<T>(result);
	}

	// 指定した要素がシーケンスに含まれているかどうかを判断
	public boolean Contains(T value){
		return _source.contains(value);
	}

	// シーケンス内の要素数を返す
	public int Count(){
		return _source.size();
	}

	// シーケンスから一意の要素を返す
	public Enumerable<T> Distinct(){
		List<T> result = new ArrayList<T>();
		for(T value : _source){
			if(!result.contains(value)){
				result.add(value);
			}
		}
		return new Enumerable<T>(result);
	}

	// 2つのシーケンスの差集合を生成
	public Enumerable<T> Except(List<T> target){
		List<T> result = new ArrayList<T>();
		for(T value : _source){
			if(!target.contains(value)){
				result.add(value);
			}
		}
		return new Enumerable<T>(result);
	}

	// シーケンスの最初の要素を返す
	@SuppressWarnings("unchecked")
	public T First(){
		if(_source.isEmpty()){
			throw new IllegalStateException();
		}
		return (T)_source.toArray()[0];
	}

	// シーケンスの最初の要素を返す。シーケンスに要素が含まれていない場合は null を返す
	@SuppressWarnings("unchecked")
	public T FirstOrNull(){
		if(_source.isEmpty()){
			return null;
		}
		return (T)_source.toArray()[0];
	}

	//
	public void ForEach(Action1<T> action){
		for(T value : _source){
			action.Invoke(value);
		}
	}

	// 2つのシーケンスの積集合を生成
	public Enumerable<T> Intersect(T[] target){
		return Intersect(Arrays.asList(target));
	}

	// 2つのシーケンスの積集合を生成
	public Enumerable<T> Intersect(Collection<T> target){
		List<T> result = new ArrayList<T>(_source);
		result.retainAll(target);
		return new Enumerable<T>(result);
	}

	// シーケンスの最後の要素を返す
	@SuppressWarnings("unchecked")
	public T Last(){
		if(_source.isEmpty()){
			throw new IllegalStateException();
		}
		return (T)_source.toArray()[_source.size() - 1];
	}

	// シーケンスの最後の要素を返す。シーケンスに要素が含まれていない場合は既定値を返す
	@SuppressWarnings("unchecked")
	public T LastOrNull(){
		if(_source.isEmpty()){
			return null;
		}
		return (T)_source.toArray()[_source.size() - 1];
	}

	// シーケンスの最大値を返す
	@SuppressWarnings("unchecked")
	public T Max(){
		T[] result = (T[])_source.toArray();
		Arrays.sort(result);
		return result[result.length - 1];
	}

	// シーケンスの最小値を返す
	@SuppressWarnings("unchecked")
	public T Min(){
		T[] result = (T[])_source.toArray();
		Arrays.sort(result);
		return result[0];
	}

	// シーケンスの要素を昇順に並べ替える
	public Enumerable<T> OrderBy(){
//		T[] result = (T[])_source.toArray();
//		Arrays.sort(result);
		// 上記の方法より30ms程高速
		List<T> result = new ArrayList<T>(_source);
		Collections.reverse(result);
		Collections.reverse(result);
		return new Enumerable<T>(result);
	}

	// シーケンスの要素を降順に並べ替える
	public Enumerable<T> OrderByDescending(){
		List<T> result = new ArrayList<T>(_source);
		Collections.reverse(result);
		return new Enumerable<T>(result);
	}

	// 指定した範囲内の整数のシーケンスを生成
	public static Enumerable<Integer> Range(int start, int count){
		List<Integer> result = new ArrayList<Integer>(count);
		for(int i = start; i < start + count; i++){
			result.add(i);
		}
		return new Enumerable<Integer>(result);
	}

	// 繰り返される 1 つの値を含むシーケンスを生成
	public static<T> Enumerable<T> Repeat(T value, int count){
		List<T> result = new ArrayList<T>(count);
		for(int i=0; i < count; i++){
			result.add(value);
		}
		return new Enumerable<T>(result);
	}

	// 繰り返される 1 つの値を含むシーケンスを生成
	public Enumerable<T> Repeat(int count){
		List<T> result = new ArrayList<T>(_source.size() * count);
		for(int i=0; i < count; i++){
			for(T value : _source){
				result.add(value);
			}
		}
		return new Enumerable<T>(result);
	}

	//
	public <TResult> Enumerable<TResult> Select(Func1<T, TResult> selector){
		List<TResult> result = new ArrayList<TResult>(_source.size());
		for(T value : _source){
			result.add(selector.Invoke(value));
		}
		return new Enumerable<TResult>(result);
	}

	// 2つのシーケンスが等しいかどうかを判断
	public boolean SequenceEqual(T[] target){
		return Arrays.equals(_source.toArray(), target);
	}

	// 2つのシーケンスが等しいかどうかを判断
	public boolean SequenceEqual(Collection<T> target){
		return Arrays.equals(_source.toArray(), target.toArray());
	}

	// シーケンスの唯一の要素を返す。シーケンス内の要素が 1 つだけではない場合は、例外をスローする。
	@SuppressWarnings("unchecked")
	public T Single(){
		int size = _source.size();
		if(size != 1){
			throw new IllegalStateException();
		}
		return (T)_source.toArray()[0];
	}

	// シーケンスの唯一の要素を返す。シーケンスが空の場合、null を返します。シーケンス内に要素が複数ある場合、例外をスローする。
	@SuppressWarnings("unchecked")
	public T SingleOrNull(){
		int size = _source.size();
		if(size > 1){
			throw new IllegalStateException();
		}
		else if(size == 0){
			return null;
		}
		return (T)_source.toArray()[0];
	}

	// シーケンス内の指定された数の要素をバイパスし、残りの要素を返す
	public Enumerable<T> Skip(int count){
		if(_source.size() < count){
			return new Enumerable<T>(new ArrayList<T>());
		}
		List<T> result = new ArrayList<T>(_source.size() - count);
		for(T value : _source){
			if(count == 0){
				result.add(value);
			}
			else{
				count--;
			}
		}
		return new Enumerable<T>(result);
	}

	// シーケンスの合計を計算
	public Number Sum(Class<? extends Number> type){
		Number n = 0;
		for(T value : _source){
			if(value instanceof Number){
				if(type == Integer.class){
					n = n.intValue() + ((Number)value).intValue();
				}
				else if(type == Long.class){
					n = n.longValue() + ((Number)value).longValue();
				}
				else if(type == Float.class){
					n = n.floatValue() + ((Number)value).floatValue();
				}
				else if(type == Double.class){
					n = n.doubleValue() + ((Number)value).doubleValue();
				}
			}
		}
		return n;
	}

	// シーケンスを ArrayList<T> に変換する
	public ArrayList<T> ToArrayList(){
		return new ArrayList<T>(_source);
	}

	// シーケンスを HashSet<T> に変換する
	public HashSet<T> ToHashSet(){
		return new HashSet<T>(_source);
	}

	// 述語に基づいて値のシーケンスをフィルタ処理する
	public Enumerable<T> Where(Func1<T, Boolean> predicate){
		List<T> result = new ArrayList<T>();
		for(T value : _source){
			if(predicate.Invoke(value)){
				result.add(value);
			}
		}
		return new Enumerable<T>(result);
	}

	@Override
	public Iterator<T> iterator(){
		return _source.iterator();
	}
}

