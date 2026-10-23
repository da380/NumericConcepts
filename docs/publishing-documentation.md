# Publishing the documentation to GitHub Pages

A note to self, written while setting up CI for NumericConcepts in August 2026.
The repository-settings half can only be done by the repository owner through
the web UI, so it is deliberately separated from the half that lives in the
repository.

The recipe transfers to any repository that generates a directory of HTML in
CI; see [For another repository](#for-another-repository) at the end for the
three lines that change.

## Where things stand

The `doxygen` job in `.github/workflows/ci.yml` already builds the
documentation on every push and pull request, with warnings fatal, and uploads
the result with `actions/upload-artifact`. That is a genuine check — it fails
if a `@param` goes missing — but the output is a zip file behind two clicks on
a run page, so nobody reads it. Publishing it costs one settings change and one
workflow.

## 1. Enable Pages (one time, in the web UI)

**Settings → Pages → Build and deployment → Source: `GitHub Actions`.**

Not *Deploy from a branch*. The branch option serves a directory committed to
the repository, which would mean checking generated HTML into git. The
Actions option serves an artifact produced by a workflow, so nothing generated
is ever committed.

There is no "create the site" step and no branch to make: the first successful
deployment creates it. Until then the Pages settings page will say the site is
not yet published, which is expected.

Two things worth knowing before starting:

- Pages is free for public repositories. For a **private** repository it needs
  GitHub Pro, Team or Enterprise, and the site can be restricted to people with
  repository access. A public repository always means a public site, even if
  the source is what you care about.
- The site lands at `https://<owner>.github.io/<repository>/` — for this one,
  `https://da380.github.io/NumericConcepts/`. One user site
  (`<owner>.github.io`) and any number of project sites can coexist.

## 2. Add the workflow

Publishing belongs in its own file rather than in `ci.yml`, for two reasons: it
needs write permissions that the rest of CI should not have, and it must run
only on the default branch, never on a pull request from a fork.

Create `.github/workflows/docs.yml`:

```yaml
name: Documentation

on:
  push:
    branches: [main]
  workflow_dispatch:

# Only this workflow gets them: deploy-pages mints an OIDC token, which is
# what id-token is for, and writes a deployment, which is what pages is for.
permissions:
  contents: read
  pages: write
  id-token: write

# Queue deployments rather than cancelling them. cancel-in-progress: true —
# the usual choice elsewhere — can abort a half-finished deploy.
concurrency:
  group: pages
  cancel-in-progress: false

jobs:
  publish:
    runs-on: ubuntu-24.04
    environment:
      name: github-pages
      url: ${{ steps.deployment.outputs.page_url }}
    steps:
      - uses: actions/checkout@v5

      - run: sudo apt-get update && sudo apt-get install -y doxygen

      - run: cmake -S . -B build -DNUMERICCONCEPTS_BUILD_DOCS=ON
      - run: cmake --build build --target numericconcepts_docs

      - uses: actions/configure-pages@v6
      - uses: actions/upload-pages-artifact@v5
        with:
          path: build/docs/html
      - id: deployment
        uses: actions/deploy-pages@v5
```

`environment: github-pages` is required by `deploy-pages`, not optional
decoration. The `url:` line makes the published address show up on the run
page and in the repository's Environments list, which saves hunting for it.

Those action versions were the current majors in August 2026
(`configure-pages` v6, `upload-pages-artifact` v5, `deploy-pages` v5,
`checkout` v7 — this repository pins v5, which is fine). Check for newer before
copying this into a new repository; they move independently of each other.

Leave the existing `doxygen` job in `ci.yml` alone. It is the pull-request
check, and it should keep running on branches that must never be able to
publish.

## 3. Verify

Push to `main`, then watch the `Documentation` workflow. On success the run
page shows the URL, and the repository sidebar grows a **github-pages**
environment linking to it. The first deployment can take a couple of minutes to
become reachable after the workflow goes green; a 404 immediately afterwards is
usually just that.

Then point at it from the README, replacing

```
in the Doxygen output built by `-DNUMERICCONCEPTS_BUILD_DOCS=ON`
```

with a link to the published site, keeping the build instruction for anyone
working offline.

## For another repository

Three lines change:

1. the packages installed (`doxygen`, or whatever generates the HTML),
2. the commands that build it,
3. `path:` — the directory containing `index.html`.

Everything else — the permissions block, the concurrency group, the
environment, the three Pages actions — is identical every time. The one-time
**Source: GitHub Actions** setting is per repository and is the step that is
easy to forget, because the workflow fails with a permissions error rather than
anything that mentions the setting.

## Gotchas

- **`.nojekyll` is not needed here.** Jekyll ignores files and directories
  beginning with an underscore, which mangles generated output — Doxygen emits
  plenty. That only applies to branch-based publishing; the artifact path
  serves files verbatim. If you ever fall back to a `gh-pages` branch, add an
  empty `.nojekyll` at its root.
- **Do not enable `CREATE_SUBDIRS` in the Doxyfile** for a published site
  without checking the output. It scatters pages into `d0/`, `d1/`, … which
  works, but makes every URL unstable across regenerations.
- **A fork's pull request cannot deploy**, by design. Restricting the workflow
  to `push` on the default branch makes that explicit rather than leaving
  contributors with a confusing red job.
- **Deleting the workflow does not unpublish the site.** Set Source back to
  *None* under Settings → Pages.
