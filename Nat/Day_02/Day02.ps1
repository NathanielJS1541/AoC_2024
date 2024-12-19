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

function Confirm-ReportSafe {
    <#
    .SYNOPSIS
    Confirm whether a report is safe based on the differences between its
    levels.

    .DESCRIPTION
    Confirm that all levels within the given report are trending in the same
    direction, and are increasing or decreasing within the "safe" limits based
    on the differences between the report levels.

    .PARAMETER report
    [int[]]
    An array containing the differences between the levels in a given report.

    .EXAMPLE
    $reportSafe = Confirm-ReportSafe -report <report_levels>

    Confirm whether a given report is safe..
    #>

    # Define the parameters that the function accepts.
    param (
        # A report containing the level differences to check.
        [Parameter(Mandatory=$true)]
        [int[]]$reportDifferences
    )

    # Get the number of ascending and descending levels in the current
    # report, counting only differences within the given limits of
    # -3 <= x <= -1 and 1 <= x <= 3. Positive differences indicate the
    # levels were descending, and negative values indicate the levels were
    # ascending.
    $descendingCount = ($reportDifferences | Where-Object { $_ -gt 0 -and $_ -le 3 }).Count
    $ascendingCount = ($reportDifferences | Where-Object { $_ -lt 0 -and $_ -ge -3 }).Count

    # Check that all level differences within the report had the same trend,
    # and were all within tolerable levels.
    $reportSafe = [System.Math]::Max($descendingCount, $ascendingCount) -eq $reportDifferences.Count

    return $reportSafe
}

function Measure-LevelDifferences {
    <#
    .SYNOPSIS
    Measure the differences between each level in a report.

    .DESCRIPTION
    Iterates over an array of reports, and returns an array of differences for
    each report. The differences are measured between each level in a report.

    .PARAMETER reports
    [int[][]]
    An array of reports, where each report is represented by an array of levels.

    .EXAMPLE
    $differences = Measure-LevelDifferences -reports <reports_from_input>

    Measure the differences between the levels in each report.
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

function Measure-SafeReports {
    <#
    .SYNOPSIS
    Measure the number of reports that are considered safe.

    .DESCRIPTION
    Measure the number of reports that are "safe" based on the differences
    between each level within the report. This ensures that all levels follow
    the same trend (i.e., all differences have the same sign) and that the
    absolute difference between two levels is between 1 and 3 (inclusive).

    The "Problem Dampener" can also be used be used to tolerate a single bad
    level in the level difference data, if ignoring it would mean the rest of
    the report is considered safe.

    .PARAMETER differences
    [int[][]]
    An array of the differences in each level within each report.

    .PARAMETER useDampener
    [bool]
    A bool indicating whether to use the "Problem Dampener" or not.

    .EXAMPLE
    $safeReports = Measure-SafeReports -differences <level_differences> -useDampener $false

    Calculate number of reports that are "safe" without using the "Problem
    Dampener".

    .EXAMPLE
    $safeReports = Measure-SafeReports -differences <level_differences> -useDampener $true

    Calculate number of reports that are "safe" using the "Problem Dampener".
    #>

    # Define the parameters that the function accepts.
    param (
        # The differences between each level in each report.
        [Parameter(Mandatory=$true)]
        [int[][]]$differences,
        [Parameter(Mandatory=$true)]
        [bool]$useDampener
    )

    # Initialise the count of safe reports to 0.
    $safeReports = 0

    # Iterate over the differences for each report.
    foreach ($reportDifferences in $differences) {
        if (Confirm-ReportSafe -reportDifferences $reportDifferences) {
            # If the report is classed as "safe" without any modification,
            # increment the number of safe reports and move to the next report.
            $safeReports++
        }
        elseif ($useDampener) {
            # If the report was not classed as safe, but the "Problem Dampener"
            # is allowed to be used, try applying it to see if the reading can
            # be classed as safe if a single value is removed.

            # Crete permutations of the original report level differences with
            # the "Problem Dampener" applied.
            $dampened = New-ProblemDampenerPermutations -differences $reportDifferences

            # After generating each permutation of the report differences, check
            # each one to see if it is now "safe".
            foreach ($permutation in $dampened) {
                if (Confirm-ReportSafe -reportDifferences $permutation) {
                    # If the report is now safe, increment the $safeReports and
                    # immediately break out of the loop. It doesn't matter which
                    # correction is applied as long as the report is now "safe".
                    $safeReports++
                    break
                }
            }
        }
    }

    # Return the total number of safe reports.
    return $safeReports
}

function New-ProblemDampenerPermutations {
    <#
    .SYNOPSIS
    Create permutations of a given set of report differences where the "Problem
    Dampener" has been applied to any unsafe values.

    .DESCRIPTION
    Iterate over the provided set of report differences, and apply the "Problem
    Dampener" whenever an unsafe level is identified. Each permutation is
    collected and returned for validation. A permutation is only returned if the
    modification would cause the value to fall back within the "safe" range.

    Note that each permutation only contains a single modification to the
    original data.

    .PARAMETER differences
    [int[]]
    An array of the differences in each level within each report.

    .OUTPUTS
    [int[][]]
    An array of permutations of the provided report level differences, which
    should be checked to see if they are safe.

    .EXAMPLE
    $dampenedPermutations = New-ProblemDampenerPermutations -differences <level_differences>

    Create permutations of the original level differences from a report which
    can be checked to see if they have become safe.
    #>

    # Define the parameters that the function accepts.
    param (
        # The differences between each level in each report.
        [Parameter(Mandatory=$true)]
        [int[]]$differences
    )

    # Initialise a blank array to store permutations of the report with
    # the "Problem Dampener" applied to different levels. This will
    # later be used to check whether applying the "Problem Dampener"
    # would allow the report to be classed as safe.
    $dampenedPermutations = @()

    # Count the number of ascending and descending levels in the report.
    $ascendingCount = ($differences | Where-Object { $_ -lt 0 }).Count
    $descendingCount = ($differences | Where-Object { $_ -gt 0 }).Count

    # If there are an equal number of ascending and descending levels, I
    # can't think of a way that removing a single value would allow the
    # report to be classed as safe...
    if ($ascendingCount -eq $descendingCount) {
        # Until proven otherwise, skip to the next report.
        return $dampenedPermutations
    }

    # Check if the general trend for levels in the report is ascending.
    $isAscending = $ascendingCount -gt $descendingCount

    # Iterate through each level difference in the report to identify
    # ones that are unsafe, and create a permutation where the unsafe
    # report is removed.
    for ($sampleIndex = 0; $sampleIndex -lt $differences.Length; $sampleIndex++) {
        # Store the original level difference at the current index.
        $difference = $differences[$sampleIndex]

        # Check if the current level difference follows the general
        # trend of the whole report.
        $valueFollowsTrend = ($isAscending -and $difference -lt 0) -or (-not $isAscending -and $difference -gt 0)

        # Check if the current level difference falls within the "safe"
        # range.
        $valueInRange = [System.Math]::Abs($difference) -ge 1 -and [System.Math]::Abs($difference) -le 3

        # If the level difference both falls within the "safe" range and
        # conforms to the general trend of the report, skip to the next
        # level difference. To save time, permutations are only
        # generated by removing unsafe level differences.
        if ($valueInRange -and $valueFollowsTrend) {
            continue
        }

        # At this point, the current level difference is unsafe. Isolate
        # it by splitting the report about the current index.

        # Create empty arrays to store the start and end of the report
        # in, split about the unsafe level difference. Not that if the
        # unsafe level difference is the first or last element, one of
        # these will remain empty.
        $reportStart = @()
        $reportEnd = @()

        # If the unsafe level difference is not the first element,
        # create a segment of the array up until (and not including) the
        # unsafe level.
        if ($sampleIndex -gt 0) {
            $reportStart = $differences[0..($sampleIndex - 1)]
        }

        # If the unsafe level difference is not the last element, create
        # a segment of the array from after (and not including) the
        # unsafe level, to the end of the array.
        if ($sampleIndex -lt ($differences.Length - 1)) {
            $reportEnd = $differences[($sampleIndex + 1)..($differences.Length - 1)]
        }

        # Create an array with the unsafe level removed.
        $trimmedReport = $reportStart + $reportEnd

        # Since the data has been processed into "level differences"
        # rather than the raw levels, each "difference" encompasses two
        # different level values. Either of which could be erroneous,
        # and lead to an unsafe difference overall. In order to ensure a
        # "safe" solution can be found if one exists, two different
        # permutations are created for each unsafe value in the report:
        # - One where the first element of the "difference" is
        #   effectively removed. This has the effect of combining the
        #   "difference" with the previous difference, or completely
        #   removing the difference if it is the first in the report.
        # - A second where the second element of the "difference" is
        #   effectively removed. This has the effect of combining the
        #   "difference" with the next difference value, or completely
        #   removing the difference if it is the last in the report.
        #
        # Note that although each unsafe value only has 2 permutations,
        # there may be multiple unsafe values in the report. Only a
        # single permutation needs to be valid for the report to be
        # "safe" when using the "Problem Dampener", so they are all
        # compiled together and checked later.

        # Create the permutation where the first level in the level
        # difference is effectively removed.
        if ($reportStart){
            # If the first array segment of the report is not empty,
            # then the removed "unsafe" level difference needs to be
            # added to the difference that came before it, to
            # effectively remove the first level in a pair that resulted
            # in the unsafe level difference.

            # Calculate the new level difference by combining the
            # removed difference value with the level difference that
            # preceded it (at the end of the first array segment).
            $newDifference = $reportStart[$reportStart.Length - 1] + $difference

            # Check the new level difference value. If it is still not
            # within the safe limits, don't even bother adding the
            # permutation to the array to check.
            if ([System.Math]::Abs($newDifference) -le 3 -and [System.Math]::Abs($newDifference) -ge 1) {
                # Create a deep copy of the $trimmedReport, as a value
                # needs to be modified to create the permutation.
                $removeFirst = $trimmedReport | ForEach-Object { $_ }

                # Set the value of the level difference before the
                # removed difference to the combined value calculated
                # above.
                $removeFirst[($sampleIndex - 1)] = $newDifference

                # Add the permutation to the array to check later.
                $dampenedPermutations += ,($removeFirst)
            }
        }
        else {
            # If the removed level difference is at the start of the
            # array, then no modification is needed after it is removed.
            $dampenedPermutations += ,($trimmedReport)
        }


        # Create the permutation where the second level in the level
        # difference is effectively removed.
        if ($reportEnd) {
            # If the second array segment of the report is not empty,
            # then the removed "unsafe" level difference needs to be
            # added to the difference that came after it, to
            # effectively remove the second level in a pair that
            # resulted in the unsafe level difference.

            # Calculate the new level difference by combining the
            # removed difference value with the level difference that
            # succeeded it (at the start of the second array segment).
            $newValue = $reportEnd[0] + $difference

            # Check the new level difference value. If it is still not
            # within the safe limits, don't even bother adding the
            # permutation to the array to check.
            if ([System.Math]::Abs($newValue) -le 3 -and [System.Math]::Abs($newValue) -ge 1) {
                # Create a deep copy of the $trimmedReport, as a value
                # needs to be modified to create the permutation.
                $removeLast = $trimmedReport | ForEach-Object { $_ }

                # Set the value of the level difference after the
                # removed difference to the combined value calculated
                # above. This will now be at the index of the element
                # that was removed.
                $removeLast[$sampleIndex] = $newValue

                # Add the permutation to the array to check later.
                $dampenedPermutations += ,($removeLast)
            }
        }
        else {
            # If the removed level difference is at the end of the
            # array, then no modification is needed after it is removed.
            $dampenedPermutations += ,($trimmedReport)
        }
    }

    # Return any permutations for testing, or an empty array if none were found.
    # Note the use of Write-Output and -NoEnumerate here, to prevent automatic
    # unwrapping of $dampenedPermutations if a single permutation was found.
    return Write-Output $dampenedPermutations -NoEnumerate
}

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

    # Use [System.IO.File]::ReadAllLines() to read all of the data from the file
    # at once rather than Get-Content as it is faster.
    $fileContents = [System.IO.File]::ReadAllLines($inputPath)

    # Loop through each line in the file and parse it as a report.
    foreach ($line in $fileContents) {
        # Each line line contains a report, and each report consists of multiple
        # reports delimited by a space. Split each report into an [int[]]
        # containing all of the levels, and add it as a complete report to the
        # $inputData array.
        #
        # - "$line.Split(' ')" takes each line and splits it into an array of
        #   strings based on the string locations.
        # - "ForEach-Object { [int]$_ }" converts each string value into an int.
        # - ",(<array>)" ensures that each report array is added as a new
        #   element of the $inputData, rather than appended as individual
        #   elements to the end of $inputData.
        $inputData += ,($line.Split(' ') | ForEach-Object { [int]$_ })
    }

    # Return the parsed input data.
    return $inputData
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

# Ensure the specified path is resolved to an absolute path. Some functions
# support relative paths and others don't, so ensuring the path is resolved
# before testing it and continuing the program ensures there are no unexpected
# errors.
$InputPath = Resolve-Path -Path $InputPath

# Test the input parameters passed in to the script.
#
# If this method returns then the inputs are valid.
Test-Parameters -inputPath $InputPath

# Parse the input data into an [int[][]] representing an array of reports.
$inputData = Read-InputData -inputPath $InputPath

# Calculate the differences between the levels of each report.
$differences = Measure-LevelDifferences -reports $inputData

# Calculate the number of safe reports without dampening any levels.
$safeReports = Measure-SafeReports -differences $differences -useDampener $false

# Calculate the number of safe reports after applying the "Problem Dampener".
$dampenedSafeReports = Measure-SafeReports -differences $differences  -useDampener $true

# Display the total number of safe reports in the console.
Write-Output "Un-dampened number of safe reports: $safeReports."

# Display the number of safe reports after applying the "Problem Dampener" in
# the console.
Write-Output "Dampened number of safe reports: $dampenedSafeReports."

# Exit with success code.
exit $Success
