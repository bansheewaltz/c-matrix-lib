awk '{
    if (NF == 3) {
        printf "\n%s\n", $0
    } else if (NF > 7) {
        printf "\t%s %s = %s %s = %s\n", $1,$7,$9,$10,$12
    } else {
        printf "%s\n", $0
    }
    }'