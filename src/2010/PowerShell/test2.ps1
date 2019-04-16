cls
Set-Variable -Name using -Option ReadOnly -Value (@{ 
	collections = "System.Collections.{0}"
	generic = "System.Collections.Generic.{0}"
	reader = "System.Diagnostics.Eventing.Reader.{0}"
	text = "System.Text.{0}"
})

$hex = { 
	param([int]$x);
	return "0x" + [Convert]::ToString($x, 16).ToUpper()
}
&$hex(65535)

$fact = {
	param($value);
	if($value -le 1) {
		return 1
	}
	else {
		return $value * (&$fact($value - 1))
	}
}
&$fact(11)


$arr = @(1..100)
$list = New-Object ($using.generic -f "List[Int32]")(,[int[]]$arr)
echo $list.gettype()
$list = $list | 
	where { $_ % 10 -eq 0 }
[Linq.Enumerable]::Sum([int[]]$list)

