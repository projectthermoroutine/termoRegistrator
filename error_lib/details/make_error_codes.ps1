#####################################
## (C) 2015 ООО "Код Безопасности"
## Проект: SNES-AV eset
## Автор: Зайцев Роман
## Создан: 11.03.2015
## Краткое описание: создание файла с кодами результатов и текстом ошибок из error_codes.mc
#####################################

if(($args | Measure-Object).Count -ne 8)
{
	Write-Host make_error_codes.ps1 error_codes.mc error_codes.h error_codes.cpp
	Write-Error 'Invalid command line argument count'
	exit -1
}

$namespace = $args[0]
$enum_class_name = $args[1]
$facility = $args[2]
$real_except_file = $args[3]
$real_codes_file = $args[4]
$src_mc_file_name = $args[5]
$dst_header_file_name = $args[6]
$dst_cpp_file_name = $args[7]

#for debugging
#$src_mc_file_name = 'error_codes.mc'
#$dst_header_file_name = '..\error_codes.h'
#$dst_cpp_file_name = 'error_codes.cpp'

Write-Host "  facility  : $facility"
Write-Host "  enum class: $($namespace)::$($enum_class_name)"
Write-Host "  Source      mc     file: $src_mc_file_name"
Write-Host "  Destination header file: $dst_header_file_name"
Write-Host "  Destination cpp    file: $dst_cpp_file_name"

$result_code_start_str = ";//  $enum_class_name::"
$language_for_cpp_file = 'English'

$message_id_str = 'MessageId'
$symbolic_name_str = 'SymbolicName'
$language_str = 'Language'
$end_of_message_str = '.'
$mc_field_delimiter = '='

$src_mc_contents = Get-Content $src_mc_file_name -Encoding Unicode

if(($src_mc_contents | Measure-Object).Count -lt 2)
{
	Write-Error "MC file is empty"
}

function get_text_after_substring([string]$in_str, [string]$deltimiter)
{
	if($deltimiter.Length -eq 0)
	{
		throw 'Delimiter is too short'
	}

	if($in_str.Length -lt $deltimiter.Length)
	{
		throw 'Input string is too short';
	}

	$start_index = $in_str.IndexOf($deltimiter);
	if($start_index -ge 0)
	{
		return $in_str.Substring($start_index+$deltimiter.Length).Trim();
	}
	else
	{
		throw "Can't find substring '$delimiter' in string '$in_str'";
	}
}

function parse_record([array]$contents)
{
	$result = New-Object -TypeName PSObject

	foreach($line_str in $contents)
	{
		$line = [string]$line_str

		if($line.StartsWith($message_id_str))
		{
			$message_id = get_text_after_substring $line $mc_field_delimiter
			$result | Add-Member -Type NoteProperty -Name 'id' -Value $message_id
		}
		else
		{
			if($line.StartsWith($symbolic_name_str))
			{
				$symbolic_name = get_text_after_substring $line $mc_field_delimiter
				$result | Add-Member -Type NoteProperty -Name 'symbolic_name' -Value $symbolic_name
			}
			else
			{
				if($line.StartsWith($result_code_start_str))
				{
					$result_code = get_text_after_substring $line $result_code_start_str
					$result | Add-Member -Type NoteProperty -Name 'enum_class_name' -Value $result_code
				}
			}
		}
	}

	for($i = 0; $i -ne ($contents.Count-1); ++$i)
	{
		$line = [string]$contents[$i]
		if($line.StartsWith($language_str))
		{
			$language = get_text_after_substring $line $mc_field_delimiter
			if($language -eq $language_for_cpp_file)
			{
				$message = [string]($contents[$i+1])
				$message = $message.TrimEnd('%0')
				$result | Add-Member -Type NoteProperty -Name 'message' -Value $message
				break;
			}
		}
	}   

	return $result
}

function items_to_header([array]$messages)
{    
	$result  = "#pragma once`r`n"
	$result += "`r`n"
	$result += "#include <string>`r`n"
	$result += "#include <vector>`r`n"
	$result += "#include <array>`r`n"
	$result += "#include <cstdint>`r`n"
	$result += "`r`n"
	$result += "namespace $($namespace)`r`n"
	$result += "{`r`n"
	$result += "`r`n"
	$result += "	enum class $($enum_class_name): std::uint32_t`r`n"
	$result += "	{`r`n"

	foreach($message in $messages)
	{
		$result += "		$($message.enum_class_name) = $($message.id),	//  $($message.symbolic_name)`r`n"
	}

	$result += "	};`r`n"
	$result += "`r`n"

	$result += "	std::string to_static_message($($enum_class_name) code);`r`n"
	$result += "`r`n"

	$result += "	std::string to_string($($enum_class_name) code, const std::vector<std::wstring>& args_as_strings = {});`r`n"
	$result += "`r`n"

	$result += "}`r`n"
	return $result
}

function items_to_cpp([array]$messages)
{
	$result_code_start = $result_code_start_str.Substring(5);

	$result += "#include <error_lib/$($real_codes_file)>`r`n"
	$result += "#include <error_lib/$($real_except_file)>`r`n"
	$result += "#include <common/string_utils.h>`r`n"
	$result += "#include <common/log_and_throw.h>`r`n"
	$result += "#include <resources/event_messages.h>`r`n"
	$result += "#include <message_localization_lib/message.h>`r`n"
	$result += "#include <algorithm>`r`n"
	$result += "`r`n"
	$result += "namespace $($namespace)`r`n"
	$result += "{`r`n"
	$result += "`r`n"
	$result += "	namespace`r`n"
	$result += "	{`r`n"
	$result += "		using table_item_t = std::pair<$($enum_class_name), const char*>;`r`n"
	$result += "		const table_item_t code_to_text_table[] =`r`n"
	$result += "		{`r`n"

	foreach($message in $messages)
	{
		$result += "			{ $result_code_start$($message.enum_class_name),	`"$($message.message)`" },`r`n"
	}

	$result += "		};`r`n"
	$result += "	};`r`n"
	$result += "`r`n"
	$result += "	std::string to_static_message($($enum_class_name) code)`r`n"
	$result += "	{`r`n"
	$result += "		const auto it_meta(std::find_if(std::begin(code_to_text_table), std::end(code_to_text_table), [code](const table_item_t& item) { return item.first == code; }));`r`n"
	$result += "		return (it_meta != std::end(code_to_text_table) ? it_meta->second : `"`");`r`n"
	$result += "	}`r`n"
	$result += "`r`n"
	$result += "	std::string to_string($($enum_class_name) code, const std::vector<std::wstring>& args_as_strings)`r`n"
	$result += "	{`r`n"
	$result += "		const localization::code_t code_int(static_cast<localization::code_t>(code));`r`n"
	$result += "`r`n"
	$result += "		std::wstring result;`r`n"
	$result += "`r`n"
	$result += "		try`r`n"
	$result += "		{`r`n"
	$result += "			result = localization::get_message_args(localization::message_id_t($($facility), code_int), args_as_strings);`r`n"
	$result += "		}`r`n"
	$result += "		catch(const std::exception&)`r`n"
	$result += "		{`r`n"
	$result += "			const std::string static_message(to_static_message(code));`r`n"
	$result += "			if (!static_message.empty())`r`n"
	$result += "				result = localization::format_string(static_message, args_as_strings);`r`n"
	$result += "		}`r`n"
	$result += "`r`n"
	$result += "		if (result.empty())`r`n"
	$result += "			LOG_AND_THROW(::common::application_exception($($namespace)::make_error_code($($namespace)::$($enum_class_name)::invalid_$($enum_class_name)))) << `"Invalid result code: `" << code_int;`r`n"
	$result += "`r`n"
	$result += "		return string_utils::convert_wchar_to_utf8(result);`r`n"
	$result += "	}`r`n"
	$result += "`r`n"
	$result += "}`r`n"
	return $result
}

$items = @()

for($i = 0; $i -ne ($src_mc_contents.Count-1);)
{
	if($src_mc_contents[$i].StartsWith($message_id_str) -and $src_mc_contents[$i+1].StartsWith($symbolic_name_str))
	{
		$last_line_index = [Math]::Min($src_mc_contents.Count-2,$i+20)
		for($j = $i+1; $j -ne $last_line_index; ++$j)
		{
			if(($src_mc_contents[$j].Trim() -eq $end_of_message_str) -and ($src_mc_contents[$j+1].Trim().Length -eq 0))
			{
				$items += @(parse_record($src_mc_contents[$i..($j-1)]))

				break
			}
		}
		$i = $j
	}
	else
	{
		++$i
	}
}

items_to_header($items) | Out-File $dst_header_file_name
items_to_cpp($items) | Out-File $dst_cpp_file_name