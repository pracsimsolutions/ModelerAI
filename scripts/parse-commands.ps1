param(
    [Parameter(Mandatory=$true)][string]$CommandsXmlPath,
    [Parameter(Mandatory=$true)][string]$OutputPath
)

$ErrorActionPreference = 'Stop'

# Load the XML
$xml = [xml](Get-Content -Path $CommandsXmlPath -Raw)

# Build the deprecated group value (group 31 = Deprecated)
$deprecatedGroup = '31'

$output = [System.Collections.Generic.List[string]]::new()
$output.Add('commandName | returnType | params | shortDescription | deprecated')
$output.Add('---')

$count = 0
foreach ($cmd in $xml.commands.command) {
    $name = $cmd.name
    if (-not $name) { continue }

    # Return type
    $retType = if ($cmd.returntype) { $cmd.returntype } else { 'void' }

    # Parameters - stored as a single string like "(num angle)" or "(treenode obj, num val)"
    $params = ''
    if ($cmd.parameters) {
        $params = $cmd.parameters.Trim()
        # Remove outer parens if present
        if ($params.StartsWith('(') -and $params.EndsWith(')')) {
            $params = $params.Substring(1, $params.Length - 2)
        }
    }

    # Short description - clean up
    $desc = ''
    if ($cmd.shortdescription) {
        $desc = ($cmd.shortdescription -replace '\|', '/') -replace '\r?\n', ' '
        $desc = $desc.Trim()
        if ($desc.Length -gt 200) { $desc = $desc.Substring(0, 200) + '...' }
    }

    # Deprecated flag - group 31 is "Deprecated"
    $deprecated = ''
    if ($cmd.gr -eq $deprecatedGroup) {
        $deprecated = '[DEPRECATED]'
    }

    $output.Add("$name | $retType | $params | $desc | $deprecated")
    $count++
}

$output | Out-File -FilePath $OutputPath -Encoding UTF8
Write-Host "  Indexed $count commands -> $OutputPath"
