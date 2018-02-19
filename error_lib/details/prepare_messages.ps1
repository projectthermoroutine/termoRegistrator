[CmdletBinding()]
Param(
  [Parameter(Mandatory=$True,Position=0)]
   [int]$StartMessageId,
	
   [Parameter(Mandatory=$True,Position=1)]
   [string[]]$MC_FilePaths
)


#for debugging
#$StartMessageId = 5000
#$MC_FilePaths = @('error_codes.mc1','us_error_codes.mc1')

Write-Host "  start message id  : $StartMessageId"
Write-Host "  Source mc files: $MC_FilePaths"


$message_id_str = 'MessageId'
$message_id_str_pattern = 'MessageId(\s+=|=)'
$mc_field_delimiter = '='

foreach($src_mc_file_name in $MC_FilePaths)
{
    $src_mc_content = Get-Content $src_mc_file_name

    for($i = 0; $i -ne ($src_mc_content.Count-1);++$i)
    {
        if($src_mc_content[$i] -match $message_id_str_pattern)
        {
            $src_mc_content[$i] = $message_id_str + "       $mc_field_delimiter $StartMessageId"
            ++$StartMessageId
        }
    }

    Set-Content $src_mc_file_name $src_mc_content -Force -Encoding Unicode
}

