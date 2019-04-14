cls
[environment]::CurrentDirectory = "J:\images\"
cd ([environment]::CurrentDirectory)

ls | %{
    if($_.PSIsDirectory) {
        continue
    }
    $fi = [system.io.fileinfo]$_
    $ext = [system.io.path]::GetExtension($fi.Name)
    $dstName = [guid]::NewGuid().ToString() + $ext
    $dstName
    $fi.MoveTo($dstName)
}