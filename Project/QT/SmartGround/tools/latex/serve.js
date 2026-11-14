const { mathjax } = require('mathjax-full/js/mathjax.js');
const { TeX } = require('mathjax-full/js/input/tex.js');
const { SVG } = require('mathjax-full/js/output/svg.js');
const { liteAdaptor } = require('mathjax-full/js/adaptors/liteAdaptor.js');
const { RegisterHTMLHandler } = require('mathjax-full/js/handlers/html.js');
const { AllPackages } = require('mathjax-full/js/input/tex/AllPackages.js');

const adaptor = liteAdaptor();
RegisterHTMLHandler(adaptor);

const tex = new TeX({ packages: AllPackages });
const svg = new SVG({ fontCache: 'none' });
const doc = mathjax.document('', { InputJax: tex, OutputJax: svg });

function renderOne(source) {
    const node = doc.convert(source, { display: true });
    let markup = adaptor.outerHTML(node);
    const viewBox = /viewBox="([^"]+)"/.exec(markup);
    let width = 0;
    let height = 0;
    if (viewBox) {
        const parts = viewBox[1].split(/\s+/).map(Number);
        width = parts[2];
        height = parts[3];
    }
    markup = markup.replace(/<mjx-container[^>]*>/, '').replace(/<\/mjx-container>/, '');
    return { svg: markup, width, height };
}

let buffer = '';

process.stdin.setEncoding('utf8');
process.stdin.on('data', (chunk) => {
    buffer += chunk;
    let index = 0;
    while (true) {
        const newline = buffer.indexOf('\n', index);
        if (newline < 0) break;
        const line = buffer.slice(index, newline).trim();
        buffer = buffer.slice(newline + 1);
        index = 0;
        if (!line) continue;
        let payload;
        try {
            const request = JSON.parse(line);
            const result = renderOne(request.src);
            payload = { id: request.id, ok: true, svg: result.svg, width: result.width, height: result.height };
        } catch (error) {
            payload = { id: null, ok: false, error: String(error && error.message ? error.message : error) };
        }
        process.stdout.write(JSON.stringify(payload) + '\n');
    }
});
