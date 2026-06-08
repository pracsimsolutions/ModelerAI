param(
    [Parameter(Mandatory=$true)][string]$ApiRootPath,
    [Parameter(Mandatory=$true)][string]$OutputPath
)

$ErrorActionPreference = 'Stop'

$output = [System.Collections.Generic.List[string]]::new()
$output.Add('Category | ClassName | MemberType | MemberName | ReturnType | Params | Description')
$output.Add('---')

$classCount = 0
$memberCount = 0

foreach ($catDir in Get-ChildItem -Path $ApiRootPath -Directory) {
    $category = $catDir.Name

    foreach ($xmlFile in Get-ChildItem -Path $catDir.FullName -Filter '*.xml') {
        try {
            $xml = [xml](Get-Content -Path $xmlFile.FullName -Raw)
            $root = $xml.FlexScriptClass
            if (-not $root) { continue }

            $className = $root.name
            if (-not $className) { $className = $xmlFile.BaseName }
            $classCount++

            # Parse Properties
            foreach ($prop in $root.Property) {
                if (-not $prop) { continue }
                $mName = $prop.name
                if (-not $mName) { continue }

                $retType = if ($prop.'return-type') { $prop.'return-type' } else { 'Variant' }

                $desc = ''
                if ($prop.Summary) {
                    $desc = ($prop.Summary.InnerText -replace '\|', '/') -replace '\r?\n', ' '
                    $desc = $desc.Trim()
                    if ($desc.Length -gt 200) { $desc = $desc.Substring(0, 200) + '...' }
                }

                $output.Add("$category | $className | property | $mName | $retType | | $desc")
                $memberCount++
            }

            # Parse Methods
            foreach ($method in $root.Method) {
                if (-not $method) { continue }
                $mName = $method.name
                if (-not $mName) { continue }

                # Get return type from Signature element
                $retType = 'void'
                $sig = $method.Signature
                if ($sig) {
                    if ($sig.'return-type') { $retType = $sig.'return-type' }

                    # Parse params from Signature/Param children
                    $paramList = @()
                    foreach ($p in $sig.Param) {
                        if (-not $p) { continue }
                        $pType = if ($p.type) { $p.type } else { 'Variant' }
                        $pName = if ($p.name) { $p.name } else { '' }
                        $paramList += ("$pType $pName").Trim()
                    }
                    $params = $paramList -join ', '
                } else {
                    $params = ''
                }

                $desc = ''
                if ($method.Summary) {
                    $desc = ($method.Summary.InnerText -replace '\|', '/') -replace '\r?\n', ' '
                    $desc = $desc.Trim()
                    if ($desc.Length -gt 200) { $desc = $desc.Substring(0, 200) + '...' }
                }

                $output.Add("$category | $className | method | $mName | $retType | $params | $desc")
                $memberCount++
            }
        } catch {
            Write-Host "  WARNING: Could not parse $($xmlFile.Name) - $($_.Exception.Message)"
        }
    }
}

$output | Out-File -FilePath $OutputPath -Encoding UTF8
Write-Host "  Indexed $classCount classes, $memberCount members -> $OutputPath"
