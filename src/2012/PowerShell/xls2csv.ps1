
cls

# 入力ファイル
$inputFile = "d:\test1.xls"
# 出力ディレクトリ
$outputDir = ("d:\output" + (get-date -f yyyy-MM-dd-HH-mm-ss))
# 出力ディレクトリ作成
[void](md $outputDir)

$HEADER_LEFT_TOP_CELL = "A1"
$xlToRight = -4161
$xlDown    = -4121
$xlApp = new-object -com Excel.Application

try
{
    $xlApp.DisplayAlerts = $false
    $xlBooks = $xlApp.Workbooks
    $xlBook = $xlBooks.Open($inputFile)
    $xlSheet = $xlBook.Worksheets.Item(1)

    $c1 = $xlSheet.Range($HEADER_LEFT_TOP_CELL)
    $c1.Select()
    $c2 = $c1.End($xlToRight)
    
    $r1 = $xlSheet.Range($HEADER_LEFT_TOP_CELL)
    $r1.Select()
    $r2 = $r1.End($xlDown)
    
    $range = $xlSheet.Range($xlSheet.Range($r1,$r2), $xlSheet.Range($c1,$c2))
    $range.Select()
    $range.Copy()
    
    $text = [System.Windows.Clipboard]::GetText()
    $text = $text -replace "`\t", ","
}
catch [Exception]
{
    $_
}
finally
{
    $xlApp.Quit()
    [void][system.runtime.interopservices.marshal]::releasecomobject($xlApp)
}

$MAX_CHANGE = 3
$key = ""
$changeCount = 0
$total = 0
$group = 0
$header = ""
# ファイル出力
$text.Split([environment]::NewLine.ToCharArray(), [StringSplitOptions]::RemoveEmptyEntries) | %{
    $values = $_.Split(',')
    $_
    # header
    if($values[0][0] -eq "#")
    {
        echo "header..."
        if($header.Length -gt 0)
        {
            $header += [Environment]::NewLine;
        }
        $header += $_
    }
    else
    {
        if(-not ($values[0] -eq $key))
        {
            $key = $values[0]
            ++$total
            ++$changeCount
        }
        $group = [Math]::Ceiling($total / $MAX_CHANGE)
        $outputFile = ("{0}\{1:0000}.csv" -f $outputDir, $group)
        write-host "output : $outputFile"
        if(-not [System.IO.File]::Exists($outputFile))
        {
            [System.IO.File]::AppendAllText($outputFile, $header + [Environment]::NewLine, [System.Text.Encoding]::GetEncoding(932))
        }
        [System.IO.File]::AppendAllText($outputFile, $_ + [Environment]::NewLine, [System.Text.Encoding]::GetEncoding(932))
        if($changeCount -eq $MAX_CHANGE)
        {
            # reset
            $changeCount = 0
        }
    }
}
