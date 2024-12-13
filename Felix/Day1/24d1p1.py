print(
    sum(
        [abs(b-a) for (a, b) in zip(
            *map(
                sorted, list(
                    map(
                        list, zip(
                            *[map(
                                int, line.strip().split("   ")
                                ) for line in open("Real.txt", "r").readlines()
                            ]
                        )
                    )
                )
            )
        )]
    )
)