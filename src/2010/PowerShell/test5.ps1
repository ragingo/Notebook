
cls


${*} = [Linq.Enumerable]
${*}::Sum([int[]]@(1..10))

#${*}::Where([int[]]@(1..10), { param($x) $x -lt 5 })




$type = [Type]("System.Collections.Generic.List[{0}]" -f "string")
$list = New-Object $type
$list.AddRange([string[]]@("a", "b", "c"))
$list.FindAll({ param($x) $x -ne "b" })
echo $list.ToString()

