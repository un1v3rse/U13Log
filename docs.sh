#!/bin/sh

hash appledoc 2>/dev/null || { echo >&2 "appledoc not installed. Get from https://github.com/tomaz/appledoc, then run install-appledoc.sh"; exit 1; }

OUTPUT_FOLDER="docs"
PROJECT_NAME="U13Log"
PROJECT_COMPANY="Universe 13"
COMPANY_ID="com.universe13"

rm -fr $OUTPUT_FOLDER
mkdir $OUTPUT_FOLDER

appledoc \
	--create-html \
	--no-create-docset \
	--no-install-docset \
	--no-publish-docset \
    --keep-undocumented-objects \
    --keep-undocumented-members \
    --search-undocumented-doc \
	--output "$OUTPUT_FOLDER" \
	--project-name "$PROJECT_NAME" \
	--project-company "$PROJECT_COMPANY" \
	--company-id "$COMPANY_ID" \
	"U13Log"

