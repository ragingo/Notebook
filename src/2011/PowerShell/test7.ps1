
cls

function Get-PropNames($obj) {
    return ($obj | Get-Member -Type Property | select Name | %{ $_.Name })
}
function Get-Properties($obj) {
    return ($obj | Get-Member -Type Property)
}
$map = @{}

function Get-XmlNameValueCollection([Xml.XmlElement]$element) {
    $element.SelectNodes("*") | %{
        if($_.GetType() -eq [Xml.XmlElement]) {
            Get-XmlNameValueCollection $_
        }
        else {
            $map."$_" = $element."$_"
        }
    }
}


Get-WinEvent -LogName Security -FilterXPath "*[System[EventID=4624 or EventID=4634]]" | %{
    $xml = [xml]$_.ToXml()
    $elm = [Xml.XmlElement]$xml.Event
    Get-XmlNameValueCollection $elm
}

echo $map