
cls

function Get-PropNames($obj) {
    return ($obj | Get-Member -Type Property | select Name | %{ $_.Name })
}
function Get-Properties($obj) {
    return ($obj | Get-Member -Type Property)
}
$map = @{}

function Get-XmlNameValueCollection([xml]$element) {
    $props = Get-Properties $element
    $props | %{
        $name = $_.Name
        $prop = $element."$name"
        echo $prop
        if($prop.GetType() -eq [Xml.XmlElement]) {
            Get-XmlNameValueCollection $prop
        }
        else {
            $map."$name" = $prop
        }
    }
}


Get-WinEvent -LogName Security -FilterXPath "*[System[EventID=4624 or EventID=4634]]" | %{
     Get-XmlNameValueCollection ([xml]$_.ToXml())
}

echo $map