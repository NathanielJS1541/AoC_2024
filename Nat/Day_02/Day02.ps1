<#
.SYNOPSIS
    Solution to day 2 of the Advent of Code 2024 implemented in PowerShell.

.DESCRIPTION
    Parses data from the specified input file and calculates the number of safe
    reports in the input data.

.PARAMETER InputPath
    Path to the input file to process. This can either be the test input or the
    full day 2 input file.

.EXAMPLE
    .\Day02.ps1 -InputFile .\input.txt

    Use the script to process the full challenge input data.

.EXAMPLE
    .\Day02.ps1 -InputFile .\test_input.txt

    Use the script to process the challenge test input data.

.NOTES
    Author: Nathaniel Struselis
    GitHub: https://github.com/NathanielJS1541
    Source: https://github.com/NathanielJS1541/AoC_2024/blob/main/Nat/Day_02/Day02.ps1
#>

# Define the parameters that the script accepts.
param (
    # The path to the input file. This is required for the script to run.
    [Parameter(Mandatory=$true)]
    [string]$InputPath
)

# ------------------------------------------------------------------------------
# |                                 Constants                                  |
# ------------------------------------------------------------------------------

# Return code values.
#
# These are stored as constants to prevent them from being changed, but must be
# scoped to the script to prevent them from persisting after the script exits.
Set-Variable -Name "Success" -Value 0 -Option Constant -Scope Script
Set-Variable -Name "ErrInvalidArgs" -Value 1 -Option Constant -Scope Script

# ------------------------------------------------------------------------------
# |                            Function Definitions                            |
# ------------------------------------------------------------------------------

function Read-InputData {
    <#
    .SYNOPSIS
    Open the input data file and parse the data.

    .DESCRIPTION
    Open the input file and parse the contents. This splits each report into an
    [int[]], so the final return is an [int[][]] containing all report.

    .PARAMETER inputPath
    The path to the input file.

    .OUTPUTS
    [int[][]]
    An array of reports from the input file. Each report consists of an array of
    individual levels.

    .EXAMPLE
    $inputData = Read-InputData -inputPath <path_to_file>

    Parse data from the input file.
    #>

    # Define the parameters that the function accepts.
    param (
        # The path to the input file.
        [Parameter(Mandatory=$true)]
        [string]$inputPath
    )

    # Initialise an empty array, which will contain the parsed input data.
    $inputData = @()

    # Iterate over each line of the input file to parse it into a report.
    Get-Content -Path $inputPath | ForEach-Object {
        # Each line line contains a report, and each report consists of multiple
        # reports delimited by a space. Split each report into an [int[]]
        # containing all of the levels, and add it as a complete report to the
        # $inputData array.
        #
        # - "$_ -split ' '" takes each line and splits it into an array of
        #   strings based on the string locations.
        # - "ForEach-Object { [int]$_ }" converts each string value into an int.
        # - ",(<array>)" ensures that each report array is added as a new
        #   element of the $inputData, rather than appended as individual
        #   elements to the end of $inputData.
        $inputData += ,($_ -split ' ' | ForEach-Object { [int]$_ })
    }

    # Return the parsed input data.
    return $inputData
}

function Get-LevelDifferences {
    <#
    .SYNOPSIS
    Get the differences between each level in a report.

    .DESCRIPTION
    Iterates over an array of reports, and returns an array of differences for
    each report. The differences are calculated between each level in a
    report.

    .PARAMETER reports
    [int[][]]
    An array of reports, where each report is represented by an array of levels.

    .EXAMPLE
    $differences = Get-Differences -reports <reports_from_input>

    Calculate the differences between each report.
    #>

    # Define the parameters that the function accepts.
    param (
        # The reports to get the differences for.
        [Parameter(Mandatory=$true)]
        [int[][]]$reports
    )

    # Initialise a blank array to store the level differences in.
    $differences = @()

    # Iterate over each report to get the levels for each report.
    foreach ($levels in $reports) {
        # Initialise a blank array to store the level differences for the
        # current report.
        $reportDifferences = @()

        # Iterate over each level in the report (excluding the last) and
        # calculate the difference between it and the next level.
        for ($levelIndex = 0; $levelIndex -lt $levels.Count - 1; $levelIndex++) {
            # Calculate the difference between the current and next level.
            $difference = $levels[$levelIndex] - $levels[$levelIndex + 1]
            # Store the difference for the current report in an array.
            $reportDifferences += $difference
        }

        # After calculating the differences for each level within the report,
        # store the entire array as a new element of the $differences array.
        $differences += ,($reportDifferences)
    }

    # Return the calculated level differences.
    return $differences
}

function Get-SafeReports {
    <#
    .SYNOPSIS
    Get the number of reports that are considered safe.

    .DESCRIPTION
    Get the number of reports that are "safe" based on the differences between
    each level within the report. This ensures that all levels follow the same
    trend (i.e., all differences have the same sign) and that the absolute
    difference between two levels is between 1 and 3 (inclusive).

    .PARAMETER differences
    [int[][]]
    An array of the differences in level within each report.

    .EXAMPLE
    $safeReports = Get-SafeReports -differences <level_differences>

    Calculate number of reports that are "safe".
    #>

    # Define the parameters that the function accepts.
    param (
        # The differences between each level in each report.
        [Parameter(Mandatory=$true)]
        [int[][]]$differences
    )

    # Initialise the count of safe reports to 0.
    $safeReports = 0

    # Iterate over the differences for each report.
    foreach ($reportDifferences in $differences) {
        # Get the number of ascending and descending levels in the current
        # report, counting only differences within the given limits of
        # -3 <= x <= -1 and 1 <= x <= 3. Positive differences indicate the
        # levels were descending, and negative values indicate the levels were
        # ascending.
        $descendingCount = ($reportDifferences | Where-Object { $_ -gt 0 -and $_ -le 3 }).Count
        $ascendingCount = ($reportDifferences | Where-Object { $_ -lt 0 -and $_ -ge -3 }).Count

        # Check that all level differences within the report had the same trend,
        # and were all within tolerable levels.
        if ([System.Math]::Max($descendingCount, $ascendingCount) -eq $reportDifferences.Count){
            # Report is considered safe, increment the $safeReports count.
            $safeReports++
        }
    }

    # Return the total number of safe reports.
    return $safeReports
}

function Test-Parameters {
    <#
    .SYNOPSIS
    Tests input parameters, and exits the script if they are invalid.

    .DESCRIPTION
    Tests that all provided input parameters are valid. If any are not, an error
    message is written using Write-Host, and the script exits with a relevant
    error code.

    If this method returns, then all inputs were valid.

    .PARAMETER inputPath
    The path to the input file.

    .EXAMPLE
    Test-Parameters -inputPath <path_to_file>

    Checks that the provided file path is valid.
    #>

    # Define the parameters that the function accepts.
    param (
        # The path to the input file.
        [Parameter(Mandatory=$true)]
        [string]$inputPath
    )

    # Check if the path is valid and points to a file.
    #
    # The "Leaf" PathType specifies that the path must point to a file (leaf of
    # the directory tree) rather than a directory (a container).
    if (-not (Test-Path -Path $inputPath -PathType Leaf)) {
        # Write error message to the console.
        Write-Host "Error: Input file '$inputPath' does not exist." -ForegroundColor Red
        # Exit the script using one of the constant error codes.
        exit $ErrInvalidArgs
    }
}

# ------------------------------------------------------------------------------
# |                                Main Script                                 |
# ------------------------------------------------------------------------------

# Test the input parameters passed in to the script.
#
# If this method returns then the inputs are valid.
Test-Parameters -inputPath $InputPath

# Parse the input data into an [int[][]] representing an array of reports.
$inputData = Read-InputData -inputPath $InputPath

# Calculate the differences between the levels of each report.
$differences = Get-LevelDifferences -reports $inputData

# Calculate the number of safe reports.
$safeReports = Get-SafeReports -differences $differences

# Display the total number of safe reports in the console.
Write-Output "Number of safe reports: $safeReports"

# Exit with success code.
exit $Success
