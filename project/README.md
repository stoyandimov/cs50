# CS50 Final Project

## Introduction
The purpose of this document is to describe an idea for a final project in the CS50 course.

## Background
Although I'm taking an "Introduction to Computer Science" course, I have several years of software development experience. Little before the beginning of the course, and during, I have been working on a little project of mine I call Listwish.

The Listwish project intends to develop an application that can serve as a present wish list for events like birthdays, Christmas and others that one could be expected to bring a present to.

### Abstract from the design document

><i>Have you been invited to a birthday party and wondered what to buys as a present? Perhaps you didn’t know the birthday boy or gal. Wouldn’t it be nice if they had shared a soft of wish list from which you could pick a present?
<br><br>
Or perhaps, you have invited some friends to your house warming party. You knew that they would bring presents and since you needed a lot of things for the house you thought of sharing a list with them so they could bring what you need as present.
Listwish intends to provide such a web application which can make sharing your wish list with your guests easier.</i>

## Project Goals
*	Help people communicate present wish lists to their peers
*	Help people who cannot choose a present with ideas
*	Keep the element of surprise when receiving a present

## Project Idea
Create a website and a mobile app (hereafter simply referred to as app) that enables birthday party hosts to provide a list of present wishes.

Hosts (wish list authors) will be able to share the wish lists with their guests (gifters) so the ones that don’t know what to buy can get ideas.

### Use case: Creating a wish list
* A registered user logs in into the Listwish app.
* User presses a “Create wish list” button
* System provides the possible settings for the user to configure.
* User fills the wish list settings and saves.
* System provide an interface via which the user can add products/services to the wish list.
* User add products/services and their quantities.
* System persists the products and displays a “Share wish list” button.

### Use case: Sharing a wish list
* A registered user logs in into the Listwish app.
* User select an existing  wish list and click “Share wish list” button.
* System provides an interface via which the author can invite gifters to see the wish list.
* Gifter receive a message (fb message, email, etc.) with a link to register and view the wish list.

### Use case: Selecting a present from the wish list
* A registered user logs in into the Listwish app.
* System displays a list of wish lists to which the user has been invited.
* User select a wish list
* System displays products and services still available in that wish list.
* System will display only products that haven’t yet been picked up by other gifters.
* User clicks on a product/service and confirms that he/she is going to bring this present.

Hereafter the product/service will not show to other gifters that can view the wish list. The wish list author will see that this product has been claimed but the system doesn't provide information about who claimed it.

## Further Project Details

Listwish will be a web based application (and later mobile app). It will consist of the following components:
* JavaScript front-end client using ReactJS and designed according to Google's "Material Design" guidlines.
* Backed API developed using Microsoft's Asp.NET Core MVC/API framework.

There is an [alpha version](https://listwish.sdimov.space/) I have setup to share my idea with friends.
