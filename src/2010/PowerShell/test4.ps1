
@("1", "2") -join ","

echo ([Type]("System.Diagnostics.{0}" -f "Process"))::GetCurrentProcess()

echo (1MB)

$value = 100

switch($value) {
	{$_ -gt 99}  { 
		echo "over 99"
	}
	100 {
		echo "value is 100 ver0"
	}
	{$_ -eq 100} {
		echo "value is 100 ver1"
		break
	}
	{$_ -eq 100} {
		echo "value is 100 ver2"
	}
}

1..100|%{ echo user$_ }

$hex = { 
	param([int]$x);
	return "0x" + [Convert]::ToString($x, 16).ToUpper()
}
&$hex(65535)
