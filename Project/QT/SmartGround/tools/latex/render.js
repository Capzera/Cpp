const fs = require('fs');
const path = require('path');
const { mathjax } = require('mathjax-full/js/mathjax.js');
const { TeX } = require('mathjax-full/js/input/tex.js');
const { SVG } = require('mathjax-full/js/output/svg.js');
const { liteAdaptor } = require('mathjax-full/js/adaptors/liteAdaptor.js');
const { RegisterHTMLHandler } = require('mathjax-full/js/handlers/html.js');
const { AllPackages } = require('mathjax-full/js/input/tex/AllPackages.js');

const inputPath = process.argv[2];
const outputPath = process.argv[3];

if (!inputPath || !outputPath) {
    console.error('usage: node render.js <input.tex> <output.json>');
    process.exit(1);
}

const adaptor = liteAdaptor();
RegisterHTMLHandler(adaptor);

const tex = new TeX({ packages: AllPackages });
const svg = new SVG({ fontCache: 'none' });
const html = mathjax.document('', { InputJax: tex, OutputJax: svg });

const source = fs.readFileSync(inputPath, 'utf8');
const lines = source.split(/\r?\n/).filter(line => line.trim().length > 0);

const formulas = lines.map((raw, index) => {
    const node = html.convert(raw.trim(), { display: true });
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
    return { index, tex: raw.trim(), svg: markup, width, height };
});

const payload = {
    generator: 'smartground-latex',
    engine: 'mathjax-svg',
    formulas
};

fs.mkdirSync(path.dirname(outputPath), { recursive: true });
fs.writeFileSync(outputPath, JSON.stringify(payload, null, 2), 'utf8');
console.log(`wrote ${formulas.length} formula(s) -> ${outputPath}`);
