#!/bin/bash

USER=tomlankhorst

# Read Password
echo -n "Github password for '$USER':"
read -s PASSWORD
echo

REMOTE=tomlankhorst

ISSUES="https://api.github.com/repos/OpenRCT2/OpenRCT2/issues?labels=Xcode%20Fix%20Required"
GET="curl -u $USER:$PASSWORD -s"

ISSUES_JSON=$($GET $ISSUES)

PR_URLS=$(echo "$ISSUES_JSON" | jq -r '.[] | .pull_request.url')

for PR_URL in $PR_URLS; do 
	PR_JSON=$($GET $PR_URL)

	PR_NO=$(echo "$PR_JSON" | jq -r '.number')
	PR_REF=$(echo "$PR_JSON" | jq -r '.head.ref')
	PR_REF_UNIQ="$PR_REF-fix"
	PR_NAME=$(echo "$PR_JSON" | jq -r '.head.repo.full_name')

	git fetch origin pull/$PR_NO/head:$PR_REF_UNIQ
	git checkout $PR_REF_UNIQ
	git diff --summary develop

	read -p "Press enter to open the xcode project"
	open OpenRCT2.xcodeproj
	read -p "Press enter when the project is edited"
	git diff 
	read -p "All ok? Press enter to add, commit, push and create PR"

	git add .
	git commit -m "Add files to xcode project\n\nUsing helper script"
	git push -u $REMOTE

	open "https://github.com/$PR_NAME/compare/$PR_REF...$USER:$PR_REF_UNIQ"
done

