<?php

// 階乗
$fact = function ($n) use(&$fact) {
	return $n < 2 ? 1 : $fact($n-1) * $n;
};

// 総和
$sum = function ($arr) use(&$sum) {
	$cur = array_shift($arr);
	$cnt = count($arr);
	return $cnt > 0 ? $cur + $sum($arr) : $cur;
};

// 集計
$aggregate = function ($arr, $exp) use(&$aggregate) {
	$cur = array_shift($arr);
	return count($arr) > 0 ? $exp($cur, $aggregate($arr, $exp)) : $cur;
};

// 組み合わせ
$combination = function($n, $m) {
	global $aggregate;
	$f = function($x, $y){ return $x * $y; };
	$ret =
		$aggregate(range(1,$n), $f) /
		($aggregate(range(1,$m), $f) *
		 $aggregate(range(1,$n-$m), $f));
	return $ret;
};

print $sum(range(1,10)) . "\n";
print $aggregate(range(1,10), function($x, $y){return $x + $y;}) . "\n";
print $fact(10) . "\n";
print $aggregate(range(1,10), function($x, $y){return $x * $y;}) . "\n";
print $combination(35,8) . "\n";

print $aggregate(
	array('aaa', 'bbb', 'ccc'),
	function($x, $y) {
		return $x . $y;
	}
) . "\n";


