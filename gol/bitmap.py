bits = [
    # 6x6 GOL seed
    "      ",
    "###   ",
    "#  ## ",
    "##  # ",
    "  ### ",
    "      ",

    # 4 frames 4x4 glider animation
    " #  ",
    "  # ",
    "### ",
    "    ",

    "    ",
    "# # ",
    " ## ",
    " #  ",

    "    ",
    "  # ",
    "# # ",
    " ## ",

    "    ",
    " #  ",
    "  ##",
    " ## ",
]

print("".join(chr(36+sum(0 if bit==" " else 1<<i for i,bit in enumerate(line))) for line in bits))
