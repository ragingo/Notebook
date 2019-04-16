

[void][System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms")

function MainForm()
{
	$this = New-Object System.Windows.Forms.Form
	
	function Initialize-Component()
	{
		$this.Size = New-Object System.Drawing.Size(500, 500)
		$this.Controls.Add($grid)
	}
	
	function ctor()
	{
		Initialize-Component
	}
	
	ctor
	return $this
}

function Main()
{
	[System.Windows.Forms.Application]::Run((MainForm))
}

Main

