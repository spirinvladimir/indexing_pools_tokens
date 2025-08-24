fs = require('fs')

function progress(current, total) {
  const percent = Math.floor((current / total) * 100)
  const bar = '█'.repeat(Math.floor(percent / 2)) + '░'.repeat(50 - Math.floor(percent / 2))
  process.stdout.write(`\r[${bar}] ${percent}%`)
}

t = {}
save = (token, index) =>
    token.split('').reduce(
        (t, l, i, a, last = i == a.length - 1) => {
            if (last) {t[l] = index}
            else {
                t[l] ??= {}
            }
            return t[l]
        },
        t
    )

load = token => {
    letters = token.split('')
    node = t
    for (let i = 0; i < letters.length; i++) {
        letter = letters[i]
        if (node[letter] == undefined) return -1
        node = node[letter]
    }
    return node
}

pairs = require('./pools_and_tokens.json').pairs

P = []
T = []

bytesPerRow = 12
totalBytes = pairs.length * bytesPerRow
buffer = Buffer.alloc(totalBytes)
offset = 0

for (let i = 0; i < pairs.length; i++) {
    pair = pairs[i]
    P[pair.pair_index] = pair.address
    it0 = load(pair.token0)
    if (it0 == -1) {
        it0 = T.length
        T[it0] = pair.token0
        save(pair.token0, it0)
    }
    it1 = load(pair.token1)
    if (it1 == -1) {
        it1 = T.length
        T[it1] = pair.token1
        save(pair.token1, it1)
    }
    buffer.writeInt32LE(pair.pair_index, offset)
    buffer.writeInt32LE(it0, offset + 4)
    buffer.writeInt32LE(it1, offset + 8)
    offset += bytesPerRow
    progress(i, pairs.length)
}
progress(pairs.length, pairs.length)

fs.writeFileSync('p2tt.bin', buffer)
fs.writeFileSync('pools.json', JSON.stringify(P), 'utf8')
fs.writeFileSync('tokens.json', JSON.stringify(T), 'utf8')
fs.writeFileSync('radix-tree-tokens.json', JSON.stringify(t), 'utf8')
