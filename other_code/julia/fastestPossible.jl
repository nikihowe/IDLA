

NNODES = 100
NRUNS = 100

function doWork()

    max_len = 0
    max_filled = 0

    # Run 100 walks on the path
    for i in 1:NNODES
        loc = 1
        length = 0
        while loc <= max_filled
            if loc == 0
                loc += 1
            else
                loc = rand() > 0.5 ? loc + 1 : loc - 1
            end
            length += 1
        end
        max_filled = loc

        if length > max_len
            max_len = length
        end
    end
    return max_len
end

lengths = []
for i in 1:NRUNS
    if i % 10 == 0
        println(i)
    end
    push!(lengths,doWork())
end
println(maximum(lengths))
println(minimum(lengths))
println(mean(lengths))


