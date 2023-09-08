local import, importGlobal = import('smake/libraryInstaller')
importGlobal('smake/gpp')
local fs = import('smake/utils/fs')

function smake.build()
    if not fs.Exists("out") then
        fs.CreateFolder("out")
    end

    standard('c++2a')

    inputr('src')
    include({ 'include' })

    output('out/svg-format')
    build()

    smake.run()
end

function smake.run()
    run('out/svg-format')
end
